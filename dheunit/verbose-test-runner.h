#pragma once

#include "runner.h"
#include <iostream>
#include <string>

namespace dhe {
namespace unit {
namespace runner {
class VerboseTestRunner {
public:
  static inline void run() {

    run_tests(std::cout); }
};
} // namespace runner
} // namespace unit
} // namespace dhe
