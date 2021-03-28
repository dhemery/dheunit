#pragma once

#include "dheunit/log.h"
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

namespace dhe {
namespace unit {
namespace log {

class VerboseLog : public Log {
public:
  VerboseLog(std::ostream &out) : Log{out} {}

  void begin(std::string const &name) override {
    write(name);
    push();
  }

  void record(std::string const &line) override { write(line); }

  void end(bool __attribute__((unused)) write = true) override { pop(); }
};
} // namespace log
} // namespace unit
} // namespace dhe
