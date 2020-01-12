#include "suite.h"

namespace dhe {
namespace unit {
  void Suite::addAfter(std::function<void()> const &body) { context->addAfter(body); }

  void Suite::addBefore(std::function<void()> const &body) { context->addBefore(body); }

  void Suite::addContext(std::string const &name, std::function<void()> const &body) {
    auto parent = context;
    context = new Context{name, parent};
    body();
    context = parent;
  }

  void Suite::addTest(std::string const &name, const std::function<void()> &body) {
    tests.emplace_back(name, body, context);
  }

  void Suite::run() {
    for (auto test : tests) {
      test.run();
    }
  }

  auto suite() -> Suite & {
    static auto theSuite = Suite{};
    return theSuite;
  }
} // namespace unit
} // namespace dhe
