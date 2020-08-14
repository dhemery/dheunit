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
    void fail() override { testFailed = true; }

    void failNow() override {
      fail();
      throw FailNowException{};
    }

    auto failed() const -> bool override { return testFailed; }

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
        if (logger.failed()) {
          failed = true;
          std::cout << "FAILED: " << test->name() << std::endl;
          for (auto const &entry : logger.entries()) {
            std::cout << "    " << entry << std::endl;
          }
        } else {
          std::cout << "PASSED: " << test->name() << std::endl;
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

  Test::Test(std::string name) : testName{std::move(name)} { suite().add(*this); }

  auto Test::runAll() -> bool { return suite().run(); }
} // namespace unit
} // namespace dhe
