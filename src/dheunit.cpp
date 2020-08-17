#include "dheunit.h"

#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace dhe {
namespace unit {

  struct Result {
    bool failed{false};
    std::vector<std::string> logs{};
  };

  struct Runner : public Tester {
    class FailNowException : public std::exception {};

    Runner(std::function<void(Tester &)> test) : test{std::move(test)} {}

    void fail() override { result.failed = true; }

    void failNow() override {
      fail();
      throw FailNowException{};
    }

    void addLogEntry(std::string entry) override { result.logs.push_back(entry); }

    auto run() -> Result {
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
      return result;
    }

  private:
    Result result{};
    std::function<void(Tester &)> test;
  };

  /**
   * RunTest runs each test passed to operator(), describes each result to std::cout, and accumulates the number of
   * failures.
   */
  struct RunTest {
    void operator()(std::pair<std::string, TestFunc> const &nameAndTest) {
      auto const name{nameAndTest.first};
      auto test{nameAndTest.second};

      auto runner = Runner{test};
      auto const result = runner.run();

      auto const *label = result.failed ? "\033[1;31mFAILED:\033[0m " : "\033[1;32mPASSED:\033[0m ";
      std::cout << label << name << std::endl;
      for (auto const &entry : result.logs) {
        std::cout << "    " << entry << std::endl;
      }

      f += result.failed ? 1 : 0;
    }

    auto failures() const -> size_t { return f; }

  private:
    size_t f{};
  };

  /**
   * RunSuite runs the tests in each suite passed to operator(), describes each result to std::cout, and accumulates the
   * number of failures.
   */
  struct RunSuite {
    void operator()(std::pair<std::string, Suite *> const &nameAndSuite) {
      auto const suiteName{nameAndSuite.first};
      auto *suite{nameAndSuite.second};

      std::vector<std::pair<std::string, TestFunc>> suiteTests{};
      suite->addTests([&suiteName, &suiteTests](std::string const &testName, TestFunc const &test) {
        suiteTests.emplace_back(suiteName + ": " + testName, test);
      });

      auto testResults = std::for_each(suiteTests.cbegin(), suiteTests.cend(), RunTest{});
      f += testResults.failures();
    }

    auto failures() const -> size_t { return f; }

  private:
    size_t f{};
  };

  struct TestRun {
    auto run() -> size_t {
      RunSuite suiteResults = std::for_each(suites.cbegin(), suites.cend(), RunSuite{});
      RunTest testResults = std::for_each(tests.cbegin(), tests.cend(), RunTest{});
      return suiteResults.failures() + testResults.failures();
    }

    void registerSuite(std::string const &name, Suite *suite) { suites[name] = suite; }

    void registerTest(std::string const &name, Test *test) {
      tests[name] = [test](Tester &t) { test->run(t); };
    }

  private:
    std::map<std::string, Suite *> suites{};
    std::map<std::string, TestFunc> tests{};
  };

  static auto testRun() -> TestRun & {
    static auto theTestRun = TestRun{};
    return theTestRun;
  }

  auto runTests() -> size_t { return testRun().run(); }

  Suite::Suite(std::string const &name) { testRun().registerSuite(name, this); }

  Test::Test(std::string const &name) { testRun().registerTest(name, this); }
} // namespace unit
} // namespace dhe
