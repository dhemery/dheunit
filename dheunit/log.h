#pragma once

#include <ostream>
#include <string>

namespace dhe {
namespace unit {
namespace log {

class Log {

public:
  // Begin a new log section.
  virtual void begin(std::string const &name) = 0;
  // Write the line to the log.
  virtual void write(std::string const &line) = 0;
  // End the current log section.
  virtual void end() = 0;
};
} // namespace log
} // namespace unit
} // namespace dhe
