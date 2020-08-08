#include "Runner.h"

#include <iostream>
#include <string>
#include <vector>

namespace dhe {
namespace unit {
  void Runner::log(const std::string &msg) {
    std::cout << msg << std::endl;
    testLogs.push_back(msg);
  }

  void Runner::error(const std::string &msg) {
    log(msg);
    fail();
  }

  void Runner::fatal(const std::string &msg) {
    log(msg);
    failNow();
  }

  void Runner::fail() { testFailed = true; }

  void Runner::failNow() {
    fail();
    // TODO: Throw something to skip the rest of the test.
  }

  auto Runner::failed() const -> bool { return testFailed; }

  auto Runner::logs() const -> std::vector<std::string> { return testLogs; }

  void Runner::run(Test *test) {
    try {
      test->run(this);
    } catch (const std::exception &e) {
      std::cout << "Caught standard exception \"" << e.what() << std::endl;
    } catch (...) {
      std::exception_ptr thrown = std::current_exception();
      std::cout << "Caught unknown exception" << std::endl;
    }
  }
} // namespace unit
} // namespace dhe
