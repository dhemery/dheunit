#include "dheunit.h"

#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace dhe {
namespace unit {

  struct Result {
    bool failed{false};
    std::vector<std::string> logs{};
  };

  struct Runner : public Tester {
    class FailNowException : public std::exception {};

    Runner(std::function<void(Tester &)> test) : test{std::move(test)} {}

    void fail() override { result.failed = true; }

    void failNow() override {
      fail();
      throw FailNowException{};
    }

    void writeLog(std::string const &entry) override { result.logs.push_back(entry); }

    auto run() -> Result {
      try {
        test(*this);
      } catch (FailNowException const &ignored) {
      } catch (char const *s) {
        error("Unexpected string exception: ", s);
      } catch (std::exception const &e) {
        error("Unexpected exception: ", e.what());
      } catch (...) {
        error("Unrecognized exception");
      }
      return result;
    }

  private:
    Result result{};
    std::function<void(Tester &)> test;
  };

  class TestRun {
  public:
    auto run() -> bool {
      auto failed{false};
      for (auto &pair : tests) {
        auto runner = Runner{pair.second};
        auto const result = runner.run();
        auto const *label = result.failed ? "FAILED: " : "PASSED: ";
        failed = true;
        std::cout << label << pair.first << std::endl;
        for (auto const &entry : result.logs) {
          std::cout << "    " << entry << std::endl;
        }
      }
      return failed;
    }

    void registerSuite(std::string const &name, Suite *suite) {
      for (const auto &test : suite->tests()) {
        tests[name + '/' + test.first] = test.second;
      }
    }

    void registerTest(std::string const &name, Test *test) {
      tests[name] = [test](Tester &t) { test->run(t); };
    }

  private:
    std::map<std::string, std::function<void(Tester &)>> tests{};
  };

  static auto testRun() -> TestRun & {
    static auto theTestRun = TestRun{};
    return theTestRun;
  }

  auto runTests() -> bool { return testRun().run(); };

  Suite::Suite(std::string const &name) { testRun().registerSuite(name, this); }

  Test::Test(std::string const &name) { testRun().registerTest(name, this); }
} // namespace unit
} // namespace dhe
