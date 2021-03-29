#pragma once

#include "log.h"
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

namespace dhe {
namespace unit {
namespace log {

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
   * Create a logger that writes to the given log.
   *
   * @param log the log to write to
   */
  Logger(Log *log) : log_{log} {}

  /**
   * Writes the string representation of each arg to the log, separated by
   * spaces.
   *
   * @param args the values to write to the log
   */
  template <typename... Args> void log(Args... args) {
    auto line = std::ostringstream{};
    write(line, args...);
    log_->record(line.str());
  }

  /**
   * Writes the format string to the log, replacing each {} with the string
   * representation of the corresponding arg.
   *
   * @param format the message format string
   * @param args the values to insert into the formatted message
   */
  template <typename... Args>
  void logf(std::string const &format, Args... args) {
    auto line = std::ostringstream{};
    writef(line, format.c_str(), args...);
    log_->record(line.str());
  }

  void begin(std::string const &name) { log_->begin(name); }

  void end(bool always) { log_->end(always); }

private:
  Log *log_;
};
} // namespace log
} // namespace unit
} // namespace dhe
