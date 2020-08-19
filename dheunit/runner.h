#pragma once
#include "test.h"

#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace dhe {
namespace unit {
namespace runner {

/**
 * A TestID identifies a test by suite name and test name.
 * A standalone test (not part of a suite) has an empty suite name.
 */
class TestID {
public:
  TestID(std::string suite_name, std::string test_name)
      : suite_name_{std::move(suite_name)}, test_name_{std::move(test_name)} {}

  auto suite_name() const -> std::string { return suite_name_; }

  auto test_name() const -> std::string { return test_name_; }

  auto operator<(TestID const &rhs) const -> bool {
    if (suite_name_ < rhs.suite_name_) {
      return true;
    }
    if (rhs.suite_name_ < suite_name_) {
      return false;
    }
    return test_name_ < rhs.test_name_;
  }

private:
  std::string const suite_name_;
  std::string const test_name_;
};

using TestLogger = std::function<void(std::string const &)>;
using TestExecution = std::function<bool(TestLogger const &)>;
using TestFilter = std::function<void(TestID const &, TestExecution const &)>;

class FailNowException : public std::exception {};

class Runner : public Tester {
public:
  Runner(TestFunc test, TestLogger log)
      : test_{std::move(test)}, log_{std::move(log)} {}

  void fail() override { passed_ = false; }

  void fail_now() override {
    fail();
    throw FailNowException{};
  }

  auto run() -> bool {
    try {
      test_(*this);
    } catch (FailNowException const &ignored) {
    } catch (char const *s) {
      error("Unexpected string exception: ", s);
    } catch (std::exception const &e) {
      error("Unexpected exception: ", e.what());
    } catch (...) {
      error("Unrecognized exception");
    }
    return passed_;
  }

private:
  void add_log_entry(std::string entry) override { log_(entry); }

  TestFunc test_;
  TestLogger log_;
  bool passed_{true};
};

/**
 * RunTest runs each test passed to operator(), describes each result to
 * std::cout, and summarizes the results.
 */
class RunTest {
public:
  RunTest(TestFilter filter) : filter_{std::move(filter)} {}

  void operator()(std::pair<TestID, TestFunc> const &name_and_test) {
    auto const id{name_and_test.first};
    auto test{name_and_test.second};
    auto execution = [&test](TestLogger const &log) -> bool {
      return Runner{test, log}.run();
    };
    filter_(id, execution);
  }

private:
  TestFilter const filter_;
};

/**
 * RunSuite runs the tests in each suite passed to operator().
 */
class RunSuite {
public:
  RunSuite(TestFilter filter) : filter_{std::move(filter)} {}

  void operator()(std::pair<TestID, Suite *> const &id_and_suite) {
    auto const suite_id{id_and_suite.first};
    auto *suite{id_and_suite.second};

    std::vector<std::pair<TestID, TestFunc>> suite_tests{};
    auto registrar = [suite_id, &suite_tests](std::string const &test_name,
                                              TestFunc const &test) {
      const TestID test_id = TestID{suite_id.suite_name(), test_name};
      suite_tests.emplace_back(test_id, test);
    };
    suite->register_tests(registrar);

    std::for_each(suite_tests.cbegin(), suite_tests.cend(), RunTest{filter_});
  }

private:
  TestFilter const filter_;
};

class TestRun {
public:
  void run(TestFilter const &filter) {
    std::for_each(suites_.cbegin(), suites_.cend(), RunSuite{filter});
    std::for_each(tests_.cbegin(), tests_.cend(), RunTest{filter});
  }

  void register_suite(std::string const &name, Suite *suite) {
    const TestID suite_id = TestID{name, ""};
    suites_[suite_id] = suite;
  }

  void register_test(std::string const &name, Test *test) {
    auto const test_id = TestID{"", name};
    tests_[test_id] = [test](Tester &t) { test->run(t); };
  }

private:
  std::map<TestID, Suite *> suites_{};
  std::map<TestID, TestFunc> tests_{};
};

static auto test_run() -> TestRun & {
  static auto the_test_run = TestRun{};
  return the_test_run;
}

static inline void log_nothing(std::string const &entry) {}

/**
 * Passes a TestID and a TestExecution for each registered test to the filter.
 * A typical filter will invoke the execution, passing it a logger to gather the
 * logs, and print the result and the logs.
 */
inline void run_tests(TestFilter const &filter) {
  return test_run().run(filter);
}
} // namespace runner

Test::Test(std::string const &name) {
  runner::test_run().register_test(name, this);
}

Suite::Suite(std::string const &name) {
  runner::test_run().register_suite(name, this);
}
} // namespace unit
} // namespace dhe