#pragma once
#include "test.h"

#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace dhe {
namespace unit {
  namespace runner {
    struct TestID {
      TestID(std::string suiteName, std::string testName) : sName{std::move(suiteName)}, tName{std::move(testName)} {}

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

    struct Result {
      Result(TestID id, bool passed, std::vector<std::string> log) :
          testID{std::move(id)}, testPassed{passed}, testLog{std::move(log)} {}

      auto id() const -> TestID { return testID; }
      auto suiteName() const -> std::string { return testID.suiteName(); }
      auto testName() const -> std::string { return testID.testName(); }
      auto passed() const -> bool { return testPassed; }
      auto log() const -> std::vector<std::string> { return testLog; }

    private:
      TestID const testID;
      bool const testPassed;
      std::vector<std::string> testLog;
    };

    using RunIDFunc = std::function<bool(TestID const &)>;
    using LogFunc = std::function<void(TestID const &, std::string const &)>;
    using ReportFunc = std::function<void(Result const &)>;

    struct Runner : public Tester {
      class FailNowException : public std::exception {};

      Runner(TestID id, TestFunc test, LogFunc log) : id{std::move(id)}, test{std::move(test)}, log{std::move(log)} {}

      void fail() override { failed = true; }

      void failNow() override {
        fail();
        throw FailNowException{};
      }

      void addLogEntry(std::string entry) override {
        log(id, entry);
        logEntries.push_back(entry);
      }

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
        return Result{id, !failed, logEntries};
      }

    private:
      TestID id;
      TestFunc test;
      LogFunc log;
      bool failed{false};
      std::vector<std::string> logEntries{};
    };

    /**
     * RunTest runs each test passed to operator(), describes each result to std::cout, and summarizes the results.
     */
    struct RunTest {
      RunTest(RunIDFunc runID, LogFunc log, ReportFunc report) :
          runID{std::move(runID)}, log{std::move(log)}, report{std::move(report)} {}

      void operator()(std::pair<TestID, TestFunc> const &nameAndTest) {
        auto const id{nameAndTest.first};
        if (!runID(id)) {
          return;
        }

        auto test{nameAndTest.second};
        auto runner = Runner{id, test, log};
        auto const result = runner.run();

        report(result);
      }

    private:
      RunIDFunc const runID;
      LogFunc const log;
      ReportFunc const report;
    };

    /**
     * RunSuite runs the tests in each suite passed to operator(), describes each result to std::cout, and summarizes
     * the results.
     */
    struct RunSuite {
      RunSuite(RunIDFunc runID, LogFunc log, ReportFunc report) :
          runID{std::move(runID)}, log{std::move(log)}, report{std::move(report)} {}

      void operator()(std::pair<TestID, Suite *> const &idAndSuite) {
        auto const suiteID{idAndSuite.first};
        if (!runID(suiteID)) {
          return;
        }
        auto *suite{idAndSuite.second};

        std::vector<std::pair<TestID, TestFunc>> suiteTests{};
        suite->addTests([suiteID, &suiteTests](std::string const &testName, TestFunc const &test) {
          const TestID testID = TestID{suiteID.suiteName(), testName};
          suiteTests.emplace_back(testID, test);
        });

        std::for_each(suiteTests.cbegin(), suiteTests.cend(), RunTest{runID, log, report});
      }

    private:
      RunIDFunc const runID;
      LogFunc const log;
      ReportFunc const report;
    };

    struct TestRun {
      void run(RunIDFunc const &filter, LogFunc const &log, ReportFunc const &report) {
        std::for_each(suites.cbegin(), suites.cend(), RunSuite{filter, log, report});
        std::for_each(tests.cbegin(), tests.cend(), RunTest{filter, log, report});
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
    void runTests(RunIDFunc const &filter, LogFunc const &log, ReportFunc const &report) {
      return testRun().run(filter, log, report);
    }
  } // namespace runner

  Test::Test(std::string const &name) { runner::testRun().registerTest(name, this); }
  Suite::Suite(std::string const &name) { runner::testRun().registerSuite(name, this); }
} // namespace unit
} // namespace dhe