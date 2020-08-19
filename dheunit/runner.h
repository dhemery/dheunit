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
struct TestID {
  TestID(std::string suiteName, std::string testName)
      : sName{std::move(suiteName)}, tName{std::move(testName)} {}

  auto suiteName() const -> std::string { return sName; }

  auto testName() const -> std::string { return tName; }

  auto operator<(TestID const &rhs) const -> bool {
    if (sName < rhs.sName) {
      return true;
    }
    if (rhs.sName < sName) {
      return false;
    }
    return tName < rhs.tName;
  }

private:
  std::string const sName;
  std::string const tName;
};

using TestLogger = std::function<void(std::string const &)>;
using TestExecution = std::function<bool(TestLogger const &)>;
using TestFilter = std::function<void(TestID const &, TestExecution const &)>;

struct Runner : public Tester {
  class FailNowException : public std::exception {};

  Runner(TestFunc test, TestLogger log)
      : test{std::move(test)}, log{std::move(log)} {}

  void fail() override { failed = true; }

  void failNow() override {
    fail();
    throw FailNowException{};
  }

  void addLogEntry(std::string entry) override { log(entry); }

  auto run() -> bool {
    try {
      test(*this);
    } catch (FailNowException const &ignored) {
    } catch (char const *s) {
      error("Unexpected string exception: ", s);
    } catch (std::exception const &e) {
      error("Unexpected exception: ", e.what());
    } catch (...) {
      error("Unrecognized exception");
    }
    return !failed;
  }

private:
  TestFunc test;
  TestLogger log;
  bool failed{false};
};

/**
 * RunTest runs each test passed to operator(), describes each result to
 * std::cout, and summarizes the results.
 */
struct RunTest {
  RunTest(TestFilter filter) : filter{std::move(filter)} {}

  void operator()(std::pair<TestID, TestFunc> const &nameAndTest) {
    auto const id{nameAndTest.first};
    auto test{nameAndTest.second};
    auto execution = [&test](TestLogger const &log) -> bool {
      return Runner{test, log}.run();
    };
    filter(id, execution);
  }

private:
  TestFilter const filter;
};

/**
 * RunSuite runs the tests in each suite passed to operator().
 */
struct RunSuite {
  RunSuite(TestFilter filter) : filter{std::move(filter)} {}

  void operator()(std::pair<TestID, Suite *> const &idAndSuite) {
    auto const suiteID{idAndSuite.first};
    auto *suite{idAndSuite.second};

    std::vector<std::pair<TestID, TestFunc>> suiteTests{};
    auto registrar = [suiteID, &suiteTests](std::string const &testName,
                                            TestFunc const &test) {
      const TestID testID = TestID{suiteID.suiteName(), testName};
      suiteTests.emplace_back(testID, test);
    };
    suite->registerTests(registrar);

    std::for_each(suiteTests.cbegin(), suiteTests.cend(), RunTest{filter});
  }

private:
  TestFilter const filter;
};

struct TestRun {
  void run(TestFilter const &filter) {
    std::for_each(suites.cbegin(), suites.cend(), RunSuite{filter});
    std::for_each(tests.cbegin(), tests.cend(), RunTest{filter});
  }

  void registerSuite(std::string const &name, Suite *suite) {
    const TestID suiteID = TestID{name, ""};
    suites[suiteID] = suite;
  }

  void registerTest(std::string const &name, Test *test) {
    auto const testID = TestID{"", name};
    tests[testID] = [test](Tester &t) { test->run(t); };
  }

private:
  std::map<TestID, Suite *> suites{};
  std::map<TestID, TestFunc> tests{};
};

static auto testRun() -> TestRun & {
  static auto theTestRun = TestRun{};
  return theTestRun;
}

/**
 * Runs all registered suites and standalone tests.
 */
void runTests(TestFilter const &filter) { return testRun().run(filter); }
} // namespace runner

Test::Test(std::string const &name) {
  runner::testRun().registerTest(name, this);
}
Suite::Suite(std::string const &name) {
  runner::testRun().registerSuite(name, this);
}
} // namespace unit
} // namespace dhe