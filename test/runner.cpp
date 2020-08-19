#include "dheunit/runner.h"

#include <algorithm>
#include <iostream>
#include <string>

using dhe::unit::runner::runTests;
using dhe::unit::runner::TestExecution;
using dhe::unit::runner::TestFilter;
using dhe::unit::runner::TestID;
using dhe::unit::runner::TestLogger;

static auto constexpr *redText = "\u001b[31m";
static auto constexpr *greenText = "\u001b[32m";
static auto constexpr *normalText = "\u001b[0m";

void log_nothing(std::string const &entry) {}

void report(TestID const &id, TestExecution const &execution) {
  auto const passed = execution(log_nothing);
  if (passed) {
    std::cout << greenText << "PASSED: ";
  } else {
    std::cout << redText << "FAILED: ";
  }
  std::cout << normalText << id.suiteName() << ": " << id.testName() << std::endl;
}

auto main() -> int { runTests(report); }
