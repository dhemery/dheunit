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

class TestResult {
public:
  TestResult(TestID id, bool failed, std::vector<std::string> log)
      : id_{std::move(id)}, failed_{failed}, log_{std::move(log)} {}
  auto id() const -> TestID const & { return id_; }
  auto failed() const -> bool { return failed_; }
  auto passed() const -> bool { return !failed(); }
  auto log() const -> std::vector<std::string> const & { return log_; }

private:
  TestID const id_;
  bool const failed_;
  std::vector<std::string> const log_;
};

template <typename C> class RunTest {
public:
  explicit RunTest(C &controller) : controller_{controller} {}

  void operator()(std::pair<TestID, TestFunc> const &id_and_test) {
    auto const id = id_and_test.first;
    if (!controller_.select_test(id)) {
      return;
    }
    auto test = id_and_test.second;
    auto const result = run(id, test);
    controller_.report(result);
  }

private:
  static inline auto run(TestID const &id, TestFunc &test) -> TestResult {
    auto log = std::vector<std::string>();
    auto tester = Tester{log};
    try {
      test(tester);
    } catch (Tester::FailNowException const &ignored) {
    } catch (char const *s) {
      tester.error("Unexpected string exception: ", s);
    } catch (std::exception const &e) {
      tester.error("Unexpected exception: ", e.what());
    } catch (...) {
      tester.error("Unrecognized exception");
    }
    return TestResult{id, tester.failed(), log};
  }

  C &controller_;
};

template <typename C> class RunSuite {
public:
  explicit RunSuite(C &controller) : controller_{controller} {}

  void operator()(std::pair<std::string, Suite *> const &id_and_suite) {
    auto const suite_id = id_and_suite.first;
    if (!controller_.select_suite(suite_id)) {
      return;
    }
    auto *suite = id_and_suite.second;

    std::vector<std::pair<TestID, TestFunc>> suite_tests{};
    auto registrar = [suite_id, &suite_tests](std::string const &test_name,
                                              TestFunc const &test) {
      auto const test_id = TestID{suite_id, test_name};
      suite_tests.emplace_back(test_id, test);
    };
    suite->register_tests(registrar);

    std::for_each(suite_tests.cbegin(), suite_tests.cend(),
                  RunTest<C>{controller_});
  }

private:
  C &controller_;
};

class TestRun {
public:
  template <typename C> void run(C &controller) {
    std::for_each(suites_.cbegin(), suites_.cend(), RunSuite<C>{controller});
    std::for_each(tests_.cbegin(), tests_.cend(), RunTest<C>{controller});
  }

  void register_suite(std::string const &name, Suite *suite) {
    suites_[name] = suite;
  }

  void register_test(std::string const &name, Test *test) {
    auto const test_id = TestID{"", name};
    tests_[test_id] = [test](Tester &t) { (*test)(t); };
  }

private:
  std::map<std::string, Suite *> suites_{};
  std::map<TestID, TestFunc> tests_{};
};

static auto test_run() -> TestRun & {
  static auto the_test_run = TestRun{};
  return the_test_run;
}

template <typename C> static inline auto run_tests(C &controller) -> int {
  test_run().run(controller);
  return controller.summarize();
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
