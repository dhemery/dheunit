#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <utility>

namespace dhe {
namespace unit {
namespace log {

class Log {
public:
  virtual void write(std::string const &) = 0;
};

class StreamLog : public Log {
public:
  StreamLog(std::ostream &out) : out_{out} {}

  void write(std::string const &line) override { out_ << line << '\n'; }

private:
  std::ostream &out_;
};

struct FormatError : public std::runtime_error {
  FormatError(char const *what) : std::runtime_error{what} {}
};

template <typename Arg> void write(std::ostream &out, Arg arg) { out << arg; }

template <typename First, typename... More>
static inline void write(std::ostream &out, First first, More... more) {
  out << first << ' ';
  write(out, more...);
}

static inline void writef(std::ostream &out, char const *format) {
  if (format == nullptr) {
    throw FormatError{"Log format error: null format"};
  }
  while (format[0] != 0) {
    if (format[0] == '{' && format[1] == '}') {
      throw FormatError{"Log format error: not enough arguments"};
    }
    out << format[0];
    format++;
  }
}

template <typename First, typename... More>
static inline void writef(std::ostream &out, char const *format, First first,
                          More... more) {
  if (format == nullptr) {
    throw FormatError{"Log format error: null format"};
  }
  while (format[0] != 0) {
    if (format[0] == '{' && format[1] == '}') {
      out << first;
      return writef(out, format + 2, more...);
    }
    out << format[0];
    format++;
  }
  throw FormatError{"Log format error: too many arguments"};
}

class Logger {
public:
  /**
   * Create a logger that writes to the given ostream.
   *
   * @param out the ostream to write to
   * @param name the logger's name
   * @param chatty if true, write all messages, even those not marked as always
   */
  Logger(std::ostream &out, std::string const &name, bool chatty)
      : Logger{nullptr, out, name, chatty, ""} {}

  /**
   * Writes the string representation of each arg to the log, separated by
   * spaces. If the logger is not chatty, it writes the message only if always
   * is true. Before writing the message, the logger writes its name and its
   * parents' names if they have not already been written.
   *
   * @param always write this message even if the logger is not chatty
   * @param args the values to write to the log
   */
  template <typename... Args> void log(bool always, Args... args) {
    if (!always && !chatty_) {
      return;
    }
    describe();
    out_ << message_prefix_;
    write(out_, args...);
    out_ << '\n';
  }

  /**
   * Writes the format string to the log, replacing each {} with the string
   * representation of the corresponding arg. If the logger is not chatty, it
   * writes the message only if always is true. Before writing the message, the
   * logger writes its name and its parents' names if they have not already been
   * written.
   *
   * @param always log this message even if the logger is not chatty
   * @param format the message format string
   * @param args the values to insert into the formatted message
   */
  template <typename... Args>
  void logf(bool always, std::string const &format, Args... args) {
    if (!always && !chatty_) {
      return;
    }
    describe();
    out_ << message_prefix_;
    writef(out_, format.c_str(), args...);
    out_ << '\n';
  }

  /**
   * Create a child logger that inherits this logger's chattiness.
   */
  auto child(std::string const &name) -> Logger { return child(name, chatty_); }

  /**
   * Create a child logger with the given chattiness.
   */
  auto child(std::string const &name, bool chatty) -> Logger {
    return Logger{this, out_, name, chatty, message_prefix_};
  }

private:
  Logger(Logger *parent, std::ostream &out, std::string name, bool chatty,
         std::string name_prefix)
      : parent_{parent}, out_{out}, name_{std::move(name)}, chatty_{chatty},
        name_prefix_{std::move(name_prefix)}, message_prefix_{name_prefix_ +
                                                              "    "} {
    if (chatty_) {
      describe();
    }
  }

  void describe() {
    if (described_) {
      return;
    }
    described_ = true;
    if (parent_ != nullptr) {
      parent_->describe();
    }
    out_ << name_prefix_ << name_ << '\n';
  }

  Logger *parent_;
  std::ostream &out_;
  std::string name_;
  bool chatty_;
  std::string name_prefix_;
  std::string message_prefix_;
  bool described_{false};
};
} // namespace log
} // namespace unit
} // namespace dhe
