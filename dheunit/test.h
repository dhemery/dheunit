#pragma once

#include <functional>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
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
  void write(First &&first, More &&...more) {
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
  void writef(char const *f, First &&first, More &&...more) {
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

class Tester;

/**
 * The type of object that can be executed as a test.
 */
using TestFunc = std::function<void(Tester &)>;

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
  template <typename... Args> void log(Args &&...args) {
    auto entry = LogEntry{};
    entry.write(std::forward<Args>(args)...);
    log_ << prefix_ << entry.str() << '\n';
  }

  /**
   * Equivalent to log(args) followed by fail().
   */
  template <typename... Args> void error(Args &&...args) {
    log(std::forward<Args>(args)...);
    fail();
  }

  /**
   * Equivalent to log(args) followed by fail_now().
   */
  template <typename... Args> void fatal(Args &&...args) {
    log(std::forward<Args>(args)...);
    fail_now();
  }

  /**
   * Writes the format string to the test's log, replacing each {} with the
   * string representation of the corresponding arg.
   */
  template <typename... Args> void logf(char const *format, Args &&...args) {
    auto entry = LogEntry{};
    entry.writef(format, std::forward<Args>(args)...);
    log(entry.str());
  };

  /**
   * Equivalent to logf(format, args) followed by fail().
   */
  template <typename... Args> void errorf(char const *format, Args &&...args) {
    logf(format, std::forward<Args>(args)...);
    fail();
  };

  /**
   * Equivalent to logf(format, args) followed by fail_now().
   */
  template <typename... Args> void fatalf(char const *format, Args &&...args) {
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

  /**
   * Indicates whether the test has been marked as failed.
   */
  auto failed() const -> bool { return failed_; }

  template <typename Subject, typename Assertion>
  void assert_that(Subject &&subject, Assertion &&assertion) {
    assertion(*this, subject);
  }

  template <typename Subject, typename Assertion>
  void assert_that(std::string const &context, Subject &&subject,
                   Assertion &&assertion) {
    auto t = Tester{context, log_, prefix_ + "    "};
    assertion(t, subject);
    if (t.failed()) {
      fail();
    }
  }

  template <typename Subject, typename Assertion>
  void assert_that_f(Subject &&subject, Assertion &&assertion) {
    assertion(*this, subject);
    if (failed()) {
      fail_now();
    }
  }

  template <typename Subject, typename Assertion>
  void assert_that_f(std::string const &context, Subject &&subject,
                     Assertion &&assertion) {
    auto t = Tester{context, log_, prefix_ + "    "};
    assertion(t, subject);
    if (t.failed()) {
      fail_now();
    }
  }

  Tester(std::string name, std::ostream &log, std::string prefix)
      : name_{std::move(name)}, log_{log}, prefix_{std::move(prefix)} {
    log << prefix_ << name_ << '\n';
  }

  /**
   * Runs the given test as a subtest of this test.
   */
  void run(std::string name, TestFunc const &test) {
    Tester t{std::move(name), log_, prefix_ + "    "};
    test(t);
    failed_ = failed_ || t.failed();
  }

private:
  bool failed_{false};
  std::string name_;
  std::ostream &log_;
  std::string prefix_;
};

/**
 * A suite of tests.
 */
class Suite {
public:
  /**
   * Constructs a test suite and registers it with the test runner.
   */
  Suite(std::string name);

  /**
   * Called by the test runner to run the suite's tests. Your suite then calls
   * t.run(name, test) to run each test. Tests can themselves run
   * t.run(name, subtest) to run subtests.
   */
  virtual void run(Tester &t) = 0;
  auto name() const -> std::string const & { return name_; }

private:
  std::string const name_;
};
} // namespace unit
} // namespace dhe
