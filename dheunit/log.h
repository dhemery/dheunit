#pragma once

#include <ostream>
#include <string>

namespace dhe {
namespace unit {
namespace log {

class Log {
public:
  virtual void begin(std::string const &name) = 0;
  virtual void record(std::string const &line) = 0;
  virtual void end(bool write) = 0;

protected:
  Log(std::ostream &out) : out_{out} {}

  void write(std::string const &line) { out_ << prefix_ << line << '\n'; }

  void push() { adjust_prefix(indent_size); }

  void pop() { adjust_prefix(-indent_size); }

private:
  void adjust_prefix(int delta) {
    prefix_length_ += delta;
    prefix_ = std::string(prefix_length_, ' ');
  }

  static auto constexpr indent_size = 4U;
  uint16_t prefix_length_ = 0;
  std::string prefix_{};
  std::ostream &out_;
};
} // namespace log
} // namespace unit
} // namespace dhe
