#pragma once

#include <functional>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

namespace dhe {
namespace unit {

  struct Tester;
  using TestFunc = std::function<void(Tester &)>;
  using TestMap = std::map<std::string, std::function<void(Tester &)>>;

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
    virtual void run(Tester &logger) = 0;
  };

  /**
   * A suite of tests.
   */
  struct Suite {
    /**
     * Constructs a test suite and registers it by name.
     */
    Suite(std::string const &name);

    /**
     * Called by the test runner to obtain the tests that make up this suite.
     * The suite will prepend its name to the name of each test.
     */
    virtual void addTests(TestMap &) = 0;
  };

  /**
   * Runs all registered suites and standalone tests.
   * @return true iff at least one test failed
   */
  extern auto runTests() -> bool;

  /**
   * Each test function receives a tester to report test failures and other information.
   * A test ends when it calls fatal() or failNow() or when it returns.
   */
  struct Tester {
    /**
     * Writes the string representation of each arg to the test's log, separated by spaces.
     */
    template <typename... Ts> void log(Ts &&... args) {
      auto entryStream = std::ostringstream{} << std::boolalpha;
      writeTo(entryStream, args...);
      writeLog(entryStream.str());
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
      auto entryStream = std::ostringstream{} << std::boolalpha;
      writefTo(entryStream, format, args...);
      writeLog(entryStream.str());
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
    virtual void writeLog(std::string const &entry) = 0;

  private:
    template <typename T> static void writeTo(std::ostream &o, T &&t) { o << t; }

    template <typename T, typename... Ts> static void writeTo(std::ostream &o, T &&t, Ts &&... ts) {
      writeTo(o, t);
      o << ' ';
      writeTo(o, ts...);
    }

    static void writefTo(std::ostream &o, char const *f) {
      if (f == nullptr) {
        return;
      }
      while (*f != 0) {
        if (*f == '{' && *++f == '}') {
          throw std::runtime_error{"dhe::unit: too few arguments for log format"};
        }
        o << *f++;
      }
    }

    template <typename T, typename... Ts> static void writefTo(std::ostream &o, char const *f, T &&t, Ts &&... ts) {
      while (f && *f) {
        if (*f == '{' && *++f == '}') {
          o << t;
          return writefTo(o, ++f, ts...);
        }
        o << *f++;
      }
      throw std::runtime_error{"dhe::unit: too many arguments for log format"};
    }
  };
} // namespace unit
} // namespace dhe
