#pragma once
#include "internal/buffered-log.h"
#include "internal/stream-log.h"
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

static inline auto run_tests(log::Level level = Level::Debug,
                             std::ostream &out = std::cerr) -> bool {
  auto failed = false;
  auto s = suites();
  auto slog = log::StreamLog{out};
  auto blog = log::BufferedLog(&slog);
  log::Log *log;
  if (level == Level::Trace) {
    log = &slog;
  } else {
    log = &blog;
  }
  auto logger = Logger{level, log};
  std::for_each(s.cbegin(), s.cend(), [&failed, &logger](Suite *suite) {
    Tester t{&logger};
    logger.begin(suite->name());
    suite->run(t);
    logger.end();
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
