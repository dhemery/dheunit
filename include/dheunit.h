#pragma once

#include <ostream>
#include <sstream>
#include <string>

namespace dhe {
namespace unit {
  class Logger {
  public:
    template <typename... Ts> void log(Ts &&... ts) {
      auto entryStream = std::ostringstream{} << std::boolalpha;
      writeTo(entryStream, ts...);
      logEntry(entryStream.str());
    }
    template <typename... Ts> void error(Ts &&... ts) {
      log(ts...);
      fail();
    }
    template <typename... Ts> void fatal(Ts &&... ts) {
      log(ts...);
      failNow();
    }
    virtual void fail() = 0;
    virtual void failNow() = 0;
    virtual auto failed() const -> bool = 0;

  protected:
    virtual void logEntry(std::string entry) = 0;

  private:
    template <typename T> static void writeTo(std::ostream &o, T &&t) { o << t; }
    template <typename T, typename... Ts> static void writeTo(std::ostream &o, T &&t, Ts &&... ts) {
      writeTo(o, t);
      writeTo(o, ts...);
    }
  };

  class Test {
  public:
    Test(std::string name);
    virtual void run(Logger &) = 0;
    auto name() -> std::string const & { return testName; }

    static auto runAll() -> bool;

  private:
    std::string const testName;
  };

} // namespace unit
} // namespace dhe
