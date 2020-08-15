#include "dheunit.h"

#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace dhe {
namespace unit {
  class FailNowException : public std::exception {};

  class TestLogger : public Logger {
  public:
    void fail() override { testFailed = true; }

    void failNow() override {
      fail();
      throw FailNowException{};
    }

    auto failed() const -> bool { return testFailed; }

    auto entries() -> std::vector<std::string> { return logEntries; }

  protected:
    void addEntry(std::string entry) override { logEntries.push_back(entry); }

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
    void add(std::string const &name, Test *test) { tests[name] = test; }

    auto run() -> bool {
      auto failed{false};
      for (auto &pair : tests) {
        auto logger = TestLogger{};
        runTest(*pair.second, logger);
        auto const *label = logger.failed() ? "FAILED: " : "PASSED: ";
        failed = true;
        std::cout << label << pair.first << std::endl;
        for (auto const &entry : logger.entries()) {
          std::cout << "    " << entry << std::endl;
        }
      }
      return failed;
    }

  private:
    std::map<std::string, Test *> tests{};
  };

  static auto suite() -> Suite & {
    static auto theSuite = Suite{};
    return theSuite;
  }

  auto runTests() -> bool { return suite().run(); };

  Test::Test(std::string const &name) { suite().add(name, this); }

  void Logger::logfTo(std::ostream &o, char const *format) {
    if (format == nullptr) {
      return;
    }
    while (*format != 0) {
      if (*format == '%' && *++format != '%') {
        throw std::runtime_error{"invalid format: missing arguments"};
      }
      o << *format;
    }
  }
} // namespace unit
} // namespace dhe
