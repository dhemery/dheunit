#pragma once

#include <ostream>
#include <string>

namespace dhe {
namespace unit {
namespace log {

class Log {
public:
  virtual void begin(std::string const &name) = 0;
  virtual void write(std::string const &line) = 0;
  virtual void end(bool write) = 0;
};
} // namespace log
} // namespace unit
} // namespace dhe
