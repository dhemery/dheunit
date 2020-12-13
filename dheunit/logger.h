#pragma once

#include <ostream>
#include <string>
#include <utility>

namespace dhe {
namespace unit {
class Logger {
public:
  Logger(std::ostream &out, std::string name, bool chatty)
      : Logger{nullptr, out, std::move(name), chatty, ""} {}

  void start() {
    if (!chatty_) {
      return;
    }
    write_name();
  }

  void log(std::string const &message) {
    if (!chatty_) {
      return;
    }
    write_message(message);
  }

  void error(std::string const &message) { write_message(message); }

  auto child(std::string const &name) -> Logger {
    return Logger{this, name, chatty_};
  }

  auto quiet_child(std::string const &name) -> Logger {
    return Logger{this, name, false};
  }

private:
  Logger(Logger *parent, std::string name, bool chatty)
      : Logger{parent, parent->out_, std::move(name), chatty,
               parent->message_prefix_} {}

  Logger(Logger *parent, std::ostream &out, std::string name, bool chatty,
         std::string name_prefix)
      : parent_{parent}, out_{out}, name_{std::move(name)}, chatty_{chatty},
        name_prefix_{std::move(name_prefix)}, message_prefix_{name_prefix_ +
                                                              "    "} {}
  void write_message(std::string const &message) {
    write_name();
    write(message_prefix_, message);
  }

  void write_name() {
    if (wrote_name_) {
      return;
    }
    wrote_name_ = true;
    if (parent_ != nullptr) {
      parent_->write_name();
    }
    write(name_prefix_, name_);
  }

  void write(std::string const &prefix, std::string const &message) {
    out_ << prefix << message << "\n";
  }

  Logger *parent_;
  std::ostream &out_;
  std::string name_;
  bool chatty_;
  std::string name_prefix_;
  std::string message_prefix_;
  bool wrote_name_{false};
};
} // namespace unit
} // namespace dhe
