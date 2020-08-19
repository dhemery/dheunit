#include "dheunit/runner.h"

#include <algorithm>
#include <iostream>
#include <string>

using dhe::unit::runner::run_tests;
using dhe::unit::runner::TestExecution;
using dhe::unit::runner::TestFilter;
using dhe::unit::runner::TestID;
using dhe::unit::runner::TestLogger;

static auto constexpr *red_text = "\u001b[31m";
static auto constexpr *green_text = "\u001b[32m";
static auto constexpr *normal_text = "\u001b[0m";

void report(TestID const &id, TestExecution const &execution) {
  auto const passed = execution(log_nothing);
  if (passed) {
    std::cout << green_text << "PASSED: ";
  } else {
    std::cout << red_text << "FAILED: ";
  }
  std::cout << normal_text << id.suite_name() << ": " << id.test_name()
            << std::endl;
}

auto main() -> int { run_tests(report); }
