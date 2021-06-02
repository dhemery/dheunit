#include <dheunit/runner.h>

#include <iostream>

using dhe::unit::runner::run_tests;

auto main() -> int { run_tests(std::cout, true); }
