#pragma once
#include "../log.h"

#include <string>
#include <utility>

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

private:
  std::string name_;
  Log *log_;
  bool announced_{false};
};
} // namespace log
} // namespace unit
} // namespace dhe
