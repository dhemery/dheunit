#include "Test.h"

#include <iostream>

namespace dhe {
namespace unit {
  Test::Test(std::string const &name) : name{name} {
    std::cout << "test " << name << " registering itself" << std::endl;
    suite().add(this);
  }

  void Suite::add(Test *test) { tests.push_back(test); }

  void Suite::run() {
    for (const auto &test : tests) {
      auto reporter = Reporter{};
      try {
        test->run(reporter);
      } catch (const std::exception &e) {
        std::cout << "Caught standard exception \"" << e.what() << std::endl;
      } catch (...) {
        std::exception_ptr thrown = std::current_exception();
        std::cout << "Caught unknown exception" << std::endl;
      }
    }
  }

  auto suite() -> Suite & {
    static auto theSuite = Suite{};
    return theSuite;
  }
  auto Reporter::failed() const -> bool { return testFailed; }
  auto Reporter::logs() const -> std::vector<std::string> { return testLogs; }
  void Reporter::log(const std::string &msg) { testLogs.push_back(msg); }
  void Reporter::error(const std::string &msg) {
    log(msg);
    fail();
  }
  void Reporter::fatal(const std::string &msg) {
    log(msg);
    failNow();
  }
  void Reporter::fail() { testFailed = true; }
  void Reporter::failNow() {
    fail();
    // TODO: Throw something to skip the rest of the test.
  }
} // namespace unit
} // namespace dhe
