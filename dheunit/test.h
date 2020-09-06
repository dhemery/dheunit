#pragma once

#include <functional>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace dhe {
namespace unit {
/**
 * Builds a log entry from one or more write operations.
 */
class LogEntry {
public:
  struct FormatError : public std::runtime_error {
    FormatError(char const *what) : std::runtime_error{what} {}
  };

  LogEntry() { os_ << std::boolalpha; }

  template <typename Arg> void write(Arg &&arg) { os_ << arg; }

  template <typename First, typename... More>
  void write(First &&first, More &&... more) {
    write(std::forward<First>(first));
    os_ << ' ';
    write(std::forward<More>(more)...);
  }

  void writef(char const *f) {
    if (f == nullptr) {
      throw FormatError{"Log format error: null format"};
    }
    while (f[0] != 0) {
      if (f[0] == '{' && f[1] == '}') {
        throw FormatError{"Log format error: not enough arguments"};
      }
      os_ << f[0];
      f++;
    }
  }

  template <typename First, typename... More>
  void writef(char const *f, First &&first, More &&... more) {
    if (f == nullptr) {
      throw FormatError{"Log format error: null format"};
    }
    while (f[0] != 0) {
      if (f[0] == '{' && f[1] == '}') {
        os_ << std::forward<First>(first);
        return writef(f + 2, std::forward<More>(more)...);
      }
      os_ << f[0];
      f++;
    }
    throw FormatError{"Log format error: too many arguments"};
  }

  auto str() const -> std::string { return os_.str(); }

private:
  std::ostringstream os_{};
};

/**
 * Each test function receives a tester to report test failures and other
 * information. A test ends when it calls fatal() or fail_now() or when it
 * returns.
 */
class Tester {
public:
  class FailNowException : public std::exception {};

  /**
   * Writes the string representation of each arg to the test's log, separated
   * by spaces.
   */
  template <typename... Args> void log(Args &&... args) {
    auto entry = LogEntry{};
    entry.write(std::forward<Args>(args)...);
    log_.push_back(entry.str());
  }

  /**
   * Equivalent to log(args) followed by fail().
   */
  template <typename... Args> void error(Args &&... args) {
    log(std::forward<Args>(args)...);
    fail();
  }

  /**
   * Equivalent to log(args) followed by fail_now().
   */
  template <typename... Args> void fatal(Args &&... args) {
    log(std::forward<Args>(args)...);
    fail_now();
  }

  /**
   * Writes the format string to the test's log, replacing each {} with the
   * string representation of the corresponding arg.
   */
  template <typename... Args> void logf(char const *format, Args &&... args) {
    auto entry = LogEntry{};
    entry.writef(format, std::forward<Args>(args)...);
    log(entry.str());
  };

  /**
   * Equivalent to logf(format, args) followed by fail().
   */
  template <typename... Args> void errorf(char const *format, Args &&... args) {
    logf(format, std::forward<Args>(args)...);
    fail();
  };

  /**
   * Equivalent to logf(format, args) followed by fail_now().
   */
  template <typename... Args> void fatalf(char const *format, Args &&... args) {
    logf(format, std::forward<Args>(args)...);
    fail_now();
  };

  /**
   * Marks the test as failed and continues executing it.
   */
  void fail() { failed_ = true; }

  /**
   * Marks the test as failed and stops executing it.
   */
  void fail_now() {
    fail();
    throw FailNowException{};
  }

  auto failed() const -> bool { return failed_; }

  template <typename Subject, typename Assertion>
  void assert_that(Subject &&subject, Assertion &&assertion) {
    assertion(*this, std::forward<Subject>(subject));
  }

  template <typename Subject, typename Assertion>
  void assert_that(std::string const & context, Subject &&subject,
                   Assertion &&assertion) {
    auto assertion_log = std::vector<std::string>{};
    auto assertion_tester = Tester{assertion_log};

    assertion(assertion_tester, std::forward<Subject>(subject));

    if (!assertion_tester.failed()) {
      return;
    }

    for (auto const &entry : assertion_log) {
      errorf("{}: {}", context, entry);
    }
  }

  template <typename Subject, typename Assertion>
  void assert_that_f(Subject &&subject, Assertion &&assertion) {
    auto assertion_tester = Tester{log_};

    assertion(assertion_tester, std::forward<Subject>(subject));

    if (!assertion_tester.failed()) {
      return;
    }
    fail_now();
  }

  template <typename Subject, typename Assertion>
  void assert_that_f(char const *context, Subject &&subject,
                     Assertion &&assertion) {
    auto assertion_log = std::vector<std::string>{};
    auto assertion_tester = Tester{assertion_log};

    assertion(assertion_tester, std::forward<Subject>(subject));

    if (!assertion_tester.failed()) {
      return;
    }

    for (auto const &entry : assertion_log) {
      logf("{}: {}", context, entry);
    }
    fail_now();
  }

  explicit Tester(std::vector<std::string> &log) : log_{log} {}

private:
  bool failed_{false};
  std::vector<std::string> &log_;
};

/**
 * A standalone test (not part of a suite).
 */
class Test {
public:
  /**
   * Constructs a test and registers it by name.
   */
  explicit Test(std::string const &name);

  /**
   * Called by the test runner to execute this test.
   */
  virtual void operator()(Tester &tester) = 0;
};

/**
 * The type of object that can be executed as a test.
 */
using TestFunc = std::function<void(Tester &)>;

/**
 * The type of function that registers a TestFunc with its suite.
 * Each suite will passed a TestRegistrar that it can use to register its tests.
 */
using TestRegistrar =
    std::function<void(std::string const &, TestFunc const &)>;

/**
 * A suite of tests.
 */
class Suite {
public:
  /**
   * Constructs a test suite and registers it by name.
   */
  explicit Suite(std::string const &name);

  /**
   * Called by the test runner to obtain the suite's tests. Your implementation
   * can call the registrar any number of times to submit tests for the suite to
   * run. The test runner prepends the suite's name to the name of each test.
   */
  virtual void register_tests(TestRegistrar) = 0;
};
} // namespace unit
} // namespace dhe
