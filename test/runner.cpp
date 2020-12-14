#include <dheunit/runner.h>

using dhe::unit::runner::run_tests;

auto main() -> int { run_tests(std::cout, false); }
