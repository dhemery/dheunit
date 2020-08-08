#include "suite/suite.h"

#include <iostream>

void runMyTest(dhe::unit::Reporter &r) { std::cout << "Ran it!" << std::endl; }

auto main() -> int {
  dhe::unit::suite()
      .test(runMyTest)
      .run();
}
