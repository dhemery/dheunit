#include "Runner.h"

#include <sstream>
#include <string>
#include <vector>

namespace dhe {
namespace unit {
  auto Runner::log() -> std::ostream & {
    testLogs.emplace_back();
    return testLogs.back();
  }

  auto Runner::error() -> std::ostream & {
    fail();
    return log();
  }

  auto Runner::fatal() -> std::ostream & {
    failNow();
    return log();
  }

  void Runner::fail() { testFailed = true; }

  void Runner::failNow() {
    fail();
    // TODO: Throw something to skip the rest of the test.
  }

  auto Runner::failed() const -> bool { return testFailed; }

  auto Runner::logs() const -> std::vector<std::string> {
    auto logStrings = std::vector<std::string>{testLogs.size()};
    for (auto const &logStream : testLogs) {
      logStrings.push_back(logStream.str());
    }
    return logStrings;
  }

  void Runner::run(Test *test) {
    try {
      test->run(this);
    } catch (const std::exception &e) {
      fatal() << "Unexpected exception: " << e.what();
    } catch (...) {
      fatal() << "Unexpected unrecognized exception";
    }
  }
} // namespace unit
} // namespace dhe
