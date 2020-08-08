#pragma once

#include <string>

namespace dhe {
namespace unit {
  class Reporter {
  public:
    virtual void log(std::string const &msg) = 0;
    virtual void error(std::string const &msg) = 0;
    virtual void fatal(std::string const &msg) = 0;
    virtual void fail() = 0;
    virtual void failNow() = 0;
  };

  class Test {
  public:
    Test(std::string name);
    virtual void run(Reporter *) = 0;

    static auto runAll() -> int;

  private:
    const std::string name;
  };

} // namespace unit
} // namespace dhe
