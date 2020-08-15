#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <utility>

namespace dhe {
namespace unit {
  class Logger {
  public:
    /**
     * Writes the string representation of each arg to the test's log.
     */
    template <typename... Ts> void log(Ts &&... args) {
      auto entryStream = std::ostringstream{} << std::boolalpha;
      logTo(entryStream, args...);
      addEntry(entryStream.str());
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
      logfTo(entryStream, format, args...);
      addEntry(entryStream.str());
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
     * Marks the test as failing and continues executing it.
     */
    virtual void fail() = 0;

    /**
     * Marks the test as failing and stops executing it.
     */
    virtual void failNow() = 0;

  protected:
    virtual void addEntry(std::string entry) = 0;

  private:
    template <typename T> static void logTo(std::ostream &o, T &&t) { o << t; }

    template <typename T, typename... Ts> static void logTo(std::ostream &o, T &&t, Ts &&... ts) {
      logTo(o, t);
      logTo(o, ts...);
    }

    static void logfTo(std::ostream &o, char const *format);

    template <typename... Ts> static void logfTo(std::ostream &o, char const *format, Ts &&... ts) {
      logTo(o, format, ts...);
    }
  };

  class Test {
  public:
    /**
     * Constructs and registers a test with the given name.
     */
    Test(const std::string &name);

    /**
     * Executes the test.
     */
    virtual void run(Logger &logger) = 0;
  };

  /**
   * Runs all registered tests.
   * @return whether one or more tests failed.
   */
  extern auto runTests() -> bool;
} // namespace unit
} // namespace dhe
