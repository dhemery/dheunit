#include "dheunit.h"

#include <iostream>

auto main() -> int {
  auto const nfailures = dhe::unit::runTests();
  if (nfailures > 0) {
    std::cout << "Failures: " << nfailures << std::endl;
    exit(1);
  }
}
