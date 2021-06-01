#pragma once

#include "../log.h"
#include <string>
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

  void write(std::string const &line) { log_->write(line); }

  void clean() {
    if (announced_) {
      log_->end();
    }
  }

private:
  std::string name_;
  Log *log_;
  bool announced_{false};
};

class BufferedLog : public Log {
public:
  BufferedLog(Log *log) : log_{log} {}

  void begin(std::string const &name) override {
    bufs_.emplace_back(name, log_);
  }

  void write(std::string const &line) override {
    for (auto &buf : bufs_) {
      buf.announce();
    }
    bufs_.back().write(line);
  }

  void end() override {
    auto buf = bufs_.back();
    buf.clean();
    bufs_.pop_back();
  }

private:
  Log *log_;
  std::vector<LogBuffer> bufs_{};
};
} // namespace log
} // namespace unit
} // namespace dhe
