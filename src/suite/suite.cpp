#include "suite.h"

#include <iostream>
#include <sstream>

namespace dhe {
namespace unit {
  void Suite::add(std::function<void(Reporter &)> const &test) { tests.push_back(test); }

  void Suite::run() {
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
