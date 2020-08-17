#include "dheunit.h"

#include <algorithm>
#include <iostream>
#include <string>

auto main() -> int {
  static auto constexpr *redText = "\033[1;31m";
  static auto constexpr *greenText = "\033[1;32m";
  static auto constexpr *normalText = "\033[1;30m";

  auto const summary = dhe::unit::runTests();

  auto const failureCount = summary.failureCount();
  auto const someTestsFailed = failureCount > 0;

  auto const *summaryColor = someTestsFailed ? redText : greenText;

  std::cout << summaryColor << failureCount << " / " << summary.testCount() << " failed";

  if (someTestsFailed) {
    auto const failures = summary.failures();
    std::for_each(failures.cbegin(), failures.cend(), [](std::string const &failure) {
      std::cout << std::endl << "    " << failure;
    });
  }

  std::cout << normalText << std::endl;
  exit(someTestsFailed ? 1 : 0);
}
