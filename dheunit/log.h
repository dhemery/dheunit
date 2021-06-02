#pragma once

#include <ostream>
#include <string>
#include <vector>

namespace dhe {
namespace unit {
namespace log {

enum class Level {
  Info = 0,
  Error = 1,
};

struct Log {
  // Report the start of the test with the given name.
  virtual void start(std::string const &name) = 0;
  // Write the line to the log if the level is at or above the log's threshold.
  virtual void write(Level level, std::string const &line) = 0;
  // End the end of the most recently started test.
  virtual void end() = 0;
};

class DefaultLog : public Log {
public:
  DefaultLog(std::ostream &out, bool verbose) : out_{out}, verbose_{verbose} {}

  void start(std::string const &name) override {
    names_.push_back(name);
    if (verbose_) {
      announce();
    }
  }

  void write(Level level, std::string const &line) override {
    if (!verbose_ && level == Level::Info) {
      return;
    }
    announce();
    out_ << prefix_ << line << '\n';
  }

  void end() override {
    if (announced_ == names_.size()) {
      indent(-1);
    }
    names_.pop_back();
  }

private:
  void announce() {
    std::for_each(names_.cbegin() + announced_, names_.cend(),
                  [this](std::string const &name) {
                    out_ << prefix_ << name << '\n';
                    indent(1);
                  });
  }

  void indent(int size) {
    announced_ += size;
    prefix_.resize(4 * announced_, ' ');
  }

  unsigned long announced_{};
  std::vector<std::string> names_{};
  std::string prefix_{};
  std::ostream &out_;
  bool const verbose_;
};

} // namespace log
} // namespace unit
} // namespace dhe
