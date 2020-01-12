#include "suite.h"

namespace dhe {
namespace unit {
  void Suite::addTest(std::shared_ptr<Test> const &test) { tests.push_back(test); }

  void Suite::run() {
    for (const auto &test : tests) {
      test->run();
    }
  }

  auto suite() -> Suite & {
    static auto theSuite = Suite{};
    return theSuite;
  }
} // namespace unit
} // namespace dhe
