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

static auto suites() -> std::vector<Suite *> & {
  static auto suites = std::vector<Suite *>{};
  return suites;
}

static inline void run_tests(std::ostream &log) {
  auto s = suites();
  std::for_each(s.cbegin(), s.cend(), [&log](Suite *suite) {
    Tester t{suite->name(), log};
    suite->run(t);
  });
}

} // namespace runner

Suite::Suite(std::string name) : name_{std::move(name)} {
  runner::suites().push_back(this);
}
} // namespace unit
} // namespace dhe
