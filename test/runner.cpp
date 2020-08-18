#include "dheunit/runner.h"

#include <algorithm>
#include <iostream>
#include <string>

using dhe::unit::runner::LogFunc;
using dhe::unit::runner::ReportFunc;
using dhe::unit::runner::Result;
using dhe::unit::runner::RunIDFunc;
using dhe::unit::runner::runTests;
using dhe::unit::runner::TestID;

static auto constexpr *redText = "\033[1;31m";
static auto constexpr *normalText = "\033[1;30m";

auto main() -> int {
  auto nfailures{0};
  auto ntests{0};

  auto const runAll = [](TestID const &/*id*/) -> bool { return true; };
  auto const logNothing = [](TestID const &id, std::string const &entry) {};

  ReportFunc printResult = [&nfailures, &ntests](Result const &result) {
    ntests++;
    if (result.passed()) {
      return;
    }

    nfailures++;
    std::cout << redText << "FAILED: " << normalText << result.suiteName() << ": " << result.testName() << std::endl;
    for (auto const &entry : result.log()) {
      std::cout << "    " << entry << std::endl;
    }
  };

  runTests(runAll, logNothing, printResult);

  return nfailures > 0 ? 1 : 0;
}
