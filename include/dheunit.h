#pragma once

#include <ostream>
#include <string>

namespace dhe {
namespace unit {
  class Reporter {
  public:
    virtual auto log() -> std::ostream & = 0;
    virtual auto error() -> std::ostream & = 0;
    virtual auto fatal() -> std::ostream & = 0;
    virtual void fail() = 0;
    virtual void failNow() = 0;
  };

  class Test {
  public:
    Test(std::string name);
    virtual void run(Reporter *) = 0;
    auto name() -> std::string const & { return testName; }

    static auto runAll() -> int;

  private:
    const std::string testName;
  };

} // namespace unit
} // namespace dhe
