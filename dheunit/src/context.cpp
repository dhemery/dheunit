#include "context.h"

#include <utility>

namespace dhe {
namespace unit {
  Context::Context(std::string name, dhe::unit::Context *parent) : name{std::move(name)}, parent{parent} {}

  void Context::addBefore(std::function<void()> const &before) { befores.push_back(before); }

  void Context::addAfter(std::function<void()> const &after) { afters.push_back(after); }

  void Context::create() {
    if (parent != nullptr) {
      parent->create();
    }
    for (auto const &before : befores) {
      before();
    }
  }

  void Context::destroy() {
    for (auto after = afters.rbegin(); after != afters.rend(); after++) {
      (*after)();
    }
    if (parent != nullptr) {
      parent->destroy();
    }
  }
} // namespace unit
} // namespace dhe
