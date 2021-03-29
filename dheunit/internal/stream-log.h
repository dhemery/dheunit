#pragma once

#include "../log.h"
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

namespace dhe {
namespace unit {
namespace log {

class StreamLog : public Log {
public:
  StreamLog(std::ostream &out) : out_{out} {}

  void begin(std::string const &name) override {
    write(name);
    adjust_prefix(indent_size);
  }

  void write(std::string const &line) override {
    out_ << prefix_ << line << '\n';
  }

  void end(bool __attribute__((unused)) write = true) override {
    adjust_prefix(-indent_size);
  }

private:
  void adjust_prefix(int delta) {
    prefix_length_ += delta;
    prefix_ = std::string(prefix_length_, ' ');
  }

  static auto constexpr indent_size = 4U;
  std::ostream &out_;
  uint16_t prefix_length_ = 0;
  std::string prefix_{};
};
} // namespace log
} // namespace unit
} // namespace dhe
