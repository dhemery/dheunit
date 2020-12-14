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
   * by spaces. If the test's logger is not chatty, it writes the message only
   * if always is true. Before writing the message, the test writes its name and
   * its parents' names if they have not already been written.
   *
   * @param always write this message even if the test's log is not chatty
   * @param args the values to write to the log
   */
  template <typename... Args> void log(bool always, Args... args) {
    logger_.log(always, args...);
  }

  /**
   * Writes the format string to the test's log, replacing each {} with the
   * string representation of the corresponding arg. If the test's logger is not
   * chatty, it writes the message only if always is true. Before writing the
   * message, the test writes its name and its parents' names if they have not
   * already been written.
   *
   * @param always log this message even if the test's log is not chatty
   * @param format the message format string
   * @param args the values to insert into the formatted message
   */
  template <typename... Args>
  void logf(bool always, std::string const &format, Args... args) {
    logger_.logf(always, format, args...);
  }

  /**
   * Equivalent to log(false, args).
   */
  template <typename... Args> void log(Args... args) { log(false, args...); }

  /**
   * Equivalent to log(false, format, args).
   */
  template <typename... Args>
  void logf(std::string const &format, Args... args) {
    logf(false, format, args...);
  }

  /**
   * Equivalent to log(true, args) followed by fail().
   */
  template <typename... Args> void error(Args... args) {
    log(true, args...);
    fail();
  }

  /**
   * Equivalent to log(true, args) followed by fail_now().
   */
  template <typename... Args> void fatal(Args... args) {
    log(true, args...);
    fail_now();
  }

  /**
   * Equivalent to logf(true, format, args) followed by fail().
   */
  template <typename... Args>
  void errorf(std::string const &format, Args... args) {
    logf(true, format, args...);
    fail();
  };

  /**
   * Equivalent to logf(true, format, args) followed by fail_now().
   */
  template <typename... Args>
  void fatalf(std::string const &format, Args... args) {
    logf(true, format, args...);
    fail_now();
  };

  /**
   * Marks the test as failed and continues executing it.
   */
  void fail() {
    failed_ = true;
    if (parent_ != nullptr) {
      parent_->fail();
    }
  }

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
   * Apply the assertion to the subject.
   */
  template <typename Subject, typename Assertion>
  void assert_that(Subject subject, Assertion assertion) {
    assertion(*this, subject);
  }

  /**
   * Apply the assertion to the subject. If debug is true or the log is chatty,
   * log the description before applying the assertion.
   */
  template <typename Subject, typename Assertion>
  void assert_that(std::string const &description, Subject subject,
                   Assertion assertion, bool debug = false) {
    auto t = Tester{this, logger_.child(description, debug)};
    assertion(t, subject);
  }

  /**
   * Apply the assertion to the subject. If the assertion marks the test as
   * failed, stop executing the test.
   */
  template <typename Subject, typename Assertion>
  void assert_that_f(Subject subject, Assertion assertion) {
    assertion(*this, subject);
    if (failed()) {
      fail_now();
    }
  }

  /**
   * Apply the assertion to the subject. If the assertion marks the test as
   * failed, stop executing the test. If debug is true or the log is chatty, log
   * the description before applying the assertion.
   */
  template <typename Subject, typename Assertion>
  void assert_that_f(std::string const &description, Subject subject,
                     Assertion assertion, bool debug = false) {
    auto t = Tester{this, logger_.child(description, debug)};
    t.assert_that_f(subject, assertion);
  }

  Tester(Logger logger) : Tester{nullptr, std::move(logger)} {}

  /**
   * Runs the given test function as a subtest of this test. The test function
   * must have a signature equivalent to: void fun(Tester &t).
   */
  template <typename TestFunc>
  void run(const std::string &name, TestFunc test_func) {
    Tester t{this, logger_.child(name)};
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
  }

private:
  bool failed_{false};
  Tester *parent_;
  Logger logger_;

  Tester(Tester *parent, Logger logger)
      : parent_{parent}, logger_{std::move(logger)} {}

  auto prefix() const -> std::string {
    if (parent_ == nullptr) {
      return "";
    }
    return parent_->prefix() + "    ";
  }
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
