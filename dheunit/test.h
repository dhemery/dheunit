#pragma once

#include "format.h"

#include <functional>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace dhe {
namespace unit {
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
   * Writes the string representation of each arg to the test's format,
   * separated by spaces.
   */
  template <typename... Args> void log(Args... args) {
    out_ << prefix();
    format::write(out_, args...);
    out_ << '\n';
  }

  /**
   * Equivalent to format(args) followed by fail().
   */
  template <typename... Args> void error(Args... args) {
    log(args...);
    fail();
  }

  /**
   * Equivalent to format(args) followed by fail_now().
   */
  template <typename... Args> void fatal(Args... args) {
    log(args...);
    fail_now();
  }

  /**
   * Writes the format string to the test's format, replacing each {} with the
   * string representation of the corresponding arg.
   */
  template <typename... Args>
  void logf(std::string const &format, Args... args) {
    format::writef(out_, (prefix() + format).c_str(), args...);
    out_ << '\n';
  };

  /**
   * Equivalent to logf(format, args) followed by fail().
   */
  template <typename... Args>
  void errorf(std::string const &format, Args... args) {
    logf(format, args...);
    fail();
  };

  /**
   * Equivalent to logf(format, args) followed by fail_now().
   */
  template <typename... Args>
  void fatalf(std::string const &format, Args... args) {
    logf(format, args...);
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

  template <typename Subject, typename Assertion>
  void assert_that(Subject subject, Assertion assertion) {
    assertion(*this, subject);
  }

  template <typename Subject, typename Assertion>
  void assert_that(std::string const &context, Subject subject,
                   Assertion assertion) {
    auto t = Tester{this, context};
    assertion(t, subject);
  }

  template <typename Subject, typename Assertion>
  void assert_that_f(Subject subject, Assertion assertion) {
    assertion(*this, subject);
    if (failed()) {
      fail_now();
    }
  }

  template <typename Subject, typename Assertion>
  void assert_that_f(std::string const &context, Subject subject,
                     Assertion assertion) {
    auto t = Tester{this, context};
    t.assert_that_f(subject, assertion);
  }

  Tester(std::string name, std::ostream &out)
      : Tester{nullptr, std::move(name), out} {}

  /**
   * Runs the given test as a subtest of this test.
   */
  void run(const std::string &name, TestFunc const &test) {
    Tester t{this, name, out_};
    try {
      test(t);
    } catch (Tester::FailNowException const &ignored) {
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
  std::string name_;
  std::ostream &out_;

  Tester(Tester *parent, std::string name)
      : Tester{parent, std::move(name), parent->out_} {}

  Tester(Tester *parent, std::string name, std::ostream &out)
      : parent_{parent}, name_{std::move(name)}, out_{out} {
    out_ << std::boolalpha;
    log(name_);
  }

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
