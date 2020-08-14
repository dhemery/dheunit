#include "dheunit.h"

#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace dhe {
namespace unit {
  class FailNowException : public std::exception {};

  class TestLogger : public Logger {
  public:
    void logf(const char *format, ...) override {
      va_list args1;
      va_start(args1, format);
      va_list args2;
      va_copy(args2, args1);
      std::vector<char> buf(1 + std::vsnprintf(nullptr, 0, format, args1));
      va_end(args1);
      std::vsnprintf(buf.data(), buf.size(), format, args2);
      va_end(args2);
      logEntry(buf.data());
    }

    void errorf(char const *format, ...) override {
      va_list args1;
      va_start(args1, format);
      va_list args2;
      va_copy(args2, args1);
      std::vector<char> buf(1 + std::vsnprintf(nullptr, 0, format, args1));
      va_end(args1);
      std::vsnprintf(buf.data(), buf.size(), format, args2);
      va_end(args2);
      logEntry(buf.data());
      fail();
    }

    void fatalf(char const *format, ...) override {
      va_list args1;
      va_start(args1, format);
      va_list args2;
      va_copy(args2, args1);
      std::vector<char> buf(1 + std::vsnprintf(nullptr, 0, format, args1));
      va_end(args1);
      std::vsnprintf(buf.data(), buf.size(), format, args2);
      va_end(args2);
      logEntry(buf.data());
      failNow();
    }

    void fail() override { testFailed = true; }

    void failNow() override {
      fail();
      throw FailNowException{};
    }

    auto failed() const -> bool { return testFailed; }

    auto entries() -> std::vector<std::string> { return logEntries; }

  protected:
    void logEntry(std::string entry) override { logEntries.push_back(entry); }

  private:
    std::vector<std::string> logEntries{};
    bool testFailed{false};
  };

  static void runTest(Test &test, TestLogger &logger) {
    try {
      test.run(logger);
    } catch (FailNowException const &ignored) {
    } catch (char const *s) {
      logger.error("Unexpected string exception: ", s);
    } catch (std::exception const &e) {
      logger.error("Unexpected exception: ", e.what());
    } catch (...) {
      logger.error("Unrecognized exception");
    }
  }

  class Suite {
  public:
    void add(Test &test) { tests.push_back(&test); }

    auto run() -> bool {
      auto failed{false};
      for (auto *test : tests) {
        auto logger = TestLogger{};
        runTest(*test, logger);
        auto const *label = logger.failed() ? "FAILED: " : "PASSED: ";
        failed = true;
        std::cout << label << test->name() << std::endl;
        for (auto const &entry : logger.entries()) {
          std::cout << "    " << entry << std::endl;
        }
      }
      return failed;
    }

  private:
    std::vector<Test *> tests{};
  };

  static auto suite() -> Suite & {
    static auto theSuite = Suite{};
    return theSuite;
  }

  auto runTests() -> bool { return suite().run(); };

  Test::Test(std::string name) : tName{std::move(name)} { suite().add(*this); }

  auto Test::name() const -> std::string const & { return tName; }
} // namespace unit
} // namespace dhe
