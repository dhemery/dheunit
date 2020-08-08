#include "suite.h"

#include <iostream>

namespace dhe {
namespace unit {
  auto Suite::test(std::function<void(Reporter &)> const &test) -> Suite & {
    std::cout << "Adding a test" << std::endl;
    tests.push_back(test);
    return *this;
  }

  void Suite::run() {
    std::cout << "Running " << tests.size() << " things" << std::endl;
    for (const auto &test : tests) {
      auto reporter = Reporter{};
      try {
        test(reporter);
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
} // namespace unit
} // namespace dhe
