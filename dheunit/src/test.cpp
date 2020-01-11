#include "test.h"

#include "context.h"

#include <utility>

namespace dhe {
namespace unit {

  Test::Test(std::string name, std::function<void()>  body, dhe::unit::Context *context) :
      name{std::move(name)}, body{std::move(body)}, context{context} {}

  void Test::run() {
    context->create();
    body();
    context->destroy();
  }
} // namespace test
} // namespace dhe
