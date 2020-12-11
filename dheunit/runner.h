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

static inline auto run_suite(std::ostream &log)
    -> std::function<void(Suite *)> {
  return [&log](Suite *suite) {
    Tester t{suite->name(), log};
    suite->run(t);
  };
}

class TestRun {
public:
  void run(std::ostream &log) {
    std::for_each(suites_.cbegin(), suites_.cend(), run_suite(log));
  }

  void register_suite(Suite *suite) { suites_.push_back(suite); }

private:
  std::vector<Suite *> suites_{};
};

static auto test_run() -> TestRun & {
  static auto the_test_run = TestRun{};
  return the_test_run;
}

static inline void run_tests(std::ostream &log) { test_run().run(log); }

} // namespace runner

Suite::Suite(std::string name) : name_{std::move(name)} {
  runner::test_run().register_suite(this);
}
} // namespace unit
} // namespace dhe
