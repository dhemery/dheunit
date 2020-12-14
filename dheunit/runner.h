#pragma once
#include "test.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace dhe {
namespace unit {
namespace runner {

static auto suites() -> std::vector<Suite *> & {
  static auto suites = std::vector<Suite *>{};
  return suites;
}

static inline auto run_tests(std::ostream &out, bool chatty) -> bool {
  auto failed = false;
  auto s = suites();
  std::for_each(s.cbegin(), s.cend(), [&failed, &out, chatty](Suite *suite) {
    Tester t{Logger{out, suite->name(), chatty}};
    suite->run(t);
    failed = failed || t.failed();
  });
  return failed;
}

} // namespace runner

Suite::Suite(std::string name) : name_{std::move(name)} {
  runner::suites().push_back(this);
}
} // namespace unit
} // namespace dhe
