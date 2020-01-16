#include "test.h"

#include <utility>

namespace dhe {
namespace unit {

  Test::Test(std::string name, std::function<void()> body, std::shared_ptr<Context> context) :
      name{std::move(name)}, body{std::move(body)}, context{std::move(context)} {}

  void Test::run() {
    context->create();
    body();
    context->destroy();
  }
  auto Test::description() const -> std::string { return context->description() + "::" + name; }
} // namespace unit
} // namespace dhe
