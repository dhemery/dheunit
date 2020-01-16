#include "context.h"

#include <utility>

namespace dhe {
namespace unit {
  Context::Context(std::string name, std::shared_ptr<Context> parent) :
      name{std::move(name)}, parent{std::move(parent)} {}

  void Context::addBefore(std::function<void()> const &before) { befores.push_back(before); }

  void Context::addAfter(std::function<void()> const &after) { afters.push_back(after); }

  void Context::create() {
    if (parent) {
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
    if (parent) {
      parent->destroy();
    }
  }
  auto Context::description() const -> std::string {
    if (parent) {
      return parent->description() + "::" + name;
    }
    return name;
  }
} // namespace unit
} // namespace dhe