#include "dheunit/verbose-test-runner.h"

using dhe::unit::runner::VerboseTestRunner;

auto main() -> int { VerboseTestRunner{}.run(); }
