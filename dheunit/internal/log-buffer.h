#pragma once
#include "../log.h"

#include <string>
#include <utility>
#include <vector>

namespace dhe {
namespace unit {
namespace log {

class LogBuffer {
public:
  LogBuffer(std::string name, Log *log) : name_{std::move(name)}, log_{log} {}

  void announce() {
    if (announced_) {
      return;
    }
    log_->begin(name_);
    announced_ = true;
  }

  void write(std::string const &line) { buffered_lines_.push_back(line); }

  void flush() {
    for (auto const &line : buffered_lines_) {
      log_->write(line);
    }
    log_->end(true);
  }

private:
  std::string name_;
  Log *log_;
  bool announced_{false};
  std::vector<std::string> buffered_lines_{};
};
} // namespace log
} // namespace unit
} // namespace dhe
