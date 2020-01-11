#include "dheunit.h"

#include "suite.h"

namespace dhe {
namespace unit {
  static auto suite() -> Suite & {
    static auto theSuite = Suite{};
    return theSuite;
  }

  void after(const std::function<void()> &body) { suite().addAfter(body); }

  void before(std::function<void()> const &body) { suite().addBefore(body); }

  auto describe(const std::string &name, std::function<void()> const &body) -> bool {
    suite().addContext(name, body);
    return true;
  }

  void it(const std::string &name, std::function<void()> const &body) { suite().addTest(name, body); }

  void run() { suite().runTests(); }
} // namespace unit
} // namespace dhe
