#pragma once
#include "test.h"

#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace dhe {
namespace unit {
namespace runner {

static auto run_suite() -> std::function<void(Suite *)> {
  return [](Suite *suite) {
    Tester t{suite->name()};
    suite->run(t);
  };
}

class TestRun {
public:
  void run() { std::for_each(suites_.cbegin(), suites_.cend(), run_suite()); }

  void register_suite(Suite *suite) { suites_.push_back(suite); }

private:
  std::vector<Suite *> suites_{};
};

static auto test_run() -> TestRun & {
  static auto the_test_run = TestRun{};
  return the_test_run;
}

static inline void run_tests() { test_run().run(); }

} // namespace runner

Suite::Suite() { runner::test_run().register_suite(this); }
} // namespace unit
} // namespace dhe
