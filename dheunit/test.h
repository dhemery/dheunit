#pragma once

#include "logger.h"

#include <stdexcept>
#include <string>
#include <utility>

namespace dhe {
namespace unit {
using log::Logger;

/**
 * Each test function receives a tester to report test failures and other
 * information. A test ends when it calls fatal() or fail_now() or when it
 * returns.
 */
class Tester {
  class FailNowException : public std::exception {};

public:
  /**
   * Writes the string representation of each arg to the test's log, separated
   * by spaces.
   *
   * @param args the values to write to the log
   */
  template <typename... Args> void log(Args... args) { logger_->log(args...); }

  /**
   * Writes the format string to the test's log, replacing each {} with the
   * string representation of the corresponding arg.
   *
   * @param format the message format string
   * @param args the values to insert into the formatted message
   */
  template <typename... Args>
  void logf(std::string const &format, Args... args) {
    logger_->logf(format, args...);
  }

  /**
   * Equivalent to log(args) followed by fail().
   */
  template <typename... Args> void error(Args... args) {
    log(fail_text, args..., normal_text);
    fail();
  }

  /**
   * Equivalent to logf(format, args) followed by fail().
   */
  template <typename... Args>
  void errorf(std::string const &format, Args... args) {
    logf(fail_text + format + normal_text, args...);
    fail();
  };

  /**
   * Equivalent to log(args) followed by fail_now().
   */
  template <typename... Args> void fatal(Args... args) {
    log(fail_text, args..., normal_text);
    fail_now();
  }

  /**
   * Equivalent to logf(format, args) followed by fail_now().
   */
  template <typename... Args>
  void fatalf(std::string const &format, Args... args) {
    logf(fail_text + format + normal_text, args...);
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

  /**
   * Runs the given test function as a subtest of this test. The test function
   * must have a signature equivalent to: void fun(Tester &t).
   */
  template <typename TestFunc>
  void run(const std::string &name, TestFunc test_func) {
    Tester t{logger_};
    logger_->begin(name);
    try {
      test_func(t);
    } catch (FailNowException const &ignored) {
    } catch (char const *s) {
      t.error("Unexpected string exception: ", s);
    } catch (std::exception const &e) {
      t.error("Unexpected exception: ", e.what());
    } catch (...) {
      t.error("Unrecognized exception");
    }
    if (t.failed()) {
      fail();
    }
    logger_->end(failed());
  }

  Tester(Logger *logger) : logger_{logger} {}

private:
  Logger *logger_;
  bool failed_{false};

  static auto constexpr *fail_text = "\u001b[31m";
  static auto constexpr *pass_text = "\u001b[32m";
  static auto constexpr *normal_text = "\u001b[0m";
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
