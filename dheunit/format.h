#pragma once

#include <iostream>

namespace dhe {
namespace unit {
namespace format {

struct FormatError : public std::runtime_error {
  FormatError(char const *what) : std::runtime_error{what} {}
};

template <typename Arg> void write(std::ostream &out, Arg arg) { out << arg; }

template <typename First, typename... More>
void write(std::ostream &out, First first, More... more) {
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
void writef(std::ostream &out, char const *format, First first, More... more) {
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
} // namespace format
} // namespace unit
} // namespace dhe
