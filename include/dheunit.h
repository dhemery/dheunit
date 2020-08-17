#pragma once

#include <functional>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace dhe {
namespace unit {

  /**
   * Accumulates a log entry string from one or more write operations.
   */
  struct LogEntry {
    struct FormatError : public std::runtime_error {
      FormatError(char const *what) : std::runtime_error{what} {}
    };

    LogEntry() { os << std::boolalpha; }

    template <typename T> void write(T &&t) { os << t; }

    template <typename T, typename... Ts> void write(T &&t, Ts &&... ts) {
      write(t);
      os << ' ';
      write(ts...);
    }

    void writef(char const *f) {
      if (f == nullptr) {
        throw FormatError{"Log format error: null format"};
      }
      while (f[0] != 0) {
        if (f[0] == '{' && f[1] == '}') {
          throw FormatError{"Log format error: not enough arguments"};
        }
        os << f[0];
        f++;
      }
    }

    template <typename T, typename... Ts> void writef(char const *f, T &&t, Ts &&... ts) {
      if (f == nullptr) {
        throw FormatError{"Log format error: null format"};
      }
      while (f[0] != 0) {
        if (f[0] == '{' && f[1] == '}') {
          os << t;
          return writef(f + 2, ts...);
        }
        os << f[0];
        f++;
      }
      throw FormatError{"Log format error: too many arguments"};
    }

    auto str() const -> std::string { return os.str(); }

  private:
    std::ostringstream os{};
  };

  /**
   * Each test function receives a tester to report test failures and other information.
   * A test ends when it calls fatal() or failNow() or when it returns.
   */
  struct Tester {
    /**
     * Writes the string representation of each arg to the test's log, separated by spaces.
     */
    template <typename... Ts> void log(Ts &&... args) {
      auto entry = LogEntry{};
      entry.write(args...);
      addLogEntry(entry.str());
    }

    /**
     * Equivalent to log(args) followed by fail().
     */
    template <typename... Ts> void error(Ts &&... args) {
      log(args...);
      fail();
    }

    /**
     * Equivalent to log(args) followed by failNow().
     */
    template <typename... Ts> void fatal(Ts &&... args) {
      log(args...);
      failNow();
    }

    /**
     * Writes the format string to the test's log, replacing each {} with the string representation of the corresponding
     * arg.
     */
    template <typename... Ts> void logf(char const *format, Ts &&... args) {
      auto entry = LogEntry{};
      entry.writef(format, args...);
      addLogEntry(entry.str());
    };

    /**
     * Equivalent to logf(format, args) followed by fail().
     */
    template <typename... Ts> void errorf(char const *format, Ts &&... args) {
      logf(format, args...);
      fail();
    };

    /**
     * Equivalent to logf(format, args) followed by failNow().
     */
    template <typename... Ts> void fatalf(char const *format, Ts &&... args) {
      logf(format, args...);
      failNow();
    };

    /**
     * Marks the test as failed and continues executing it.
     */
    virtual void fail() = 0;

    /**
     * Marks the test as failed and stops executing it.
     */
    virtual void failNow() = 0;

  protected:
    virtual void addLogEntry(std::string entry) = 0;
  };

  /**
   * A standalone test (not part of a suite).
   */
  struct Test {
    /**
     * Constructs a test and registers it by name.
     */
    Test(std::string const &name);

    /**
     * Called by the test runner to execute this test.
     */
    virtual void run(Tester &tester) = 0;
  };

  /**
   * The type of function that can be run as a test as part of a suite.
   */
  using TestFunc = std::function<void(Tester &)>;

  /**
   * The type of function that adds a TestFunc to a suite.
   */
  using AddTestFunc = std::function<void(std::string const &, TestFunc const &)>;

  /**
   * A suite of tests.
   */
  struct Suite {
    /**
     * Constructs a test suite and registers it by name.
     */
    Suite(std::string const &name);

    /**
     * Called by the test runner to obtain the suite's tests.
     * Your implementation can call the given function any number of times to add tests to the suite.
     * The test runner prepends the suite's name to the name of each test.
     */
    virtual void addTests(AddTestFunc) = 0;
  };

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

  /**
   * Runs all registered suites and standalone tests.
   */
  extern void runTests(RunIDFunc const &runID, LogFunc const &log, ReportFunc const &report);
} // namespace unit
} // namespace dhe
