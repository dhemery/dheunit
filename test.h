#pragma once

#include "context.h"

#include <functional>
#include <string>

namespace dhe {
namespace unit {
  class Test {
  public:
    Test(std::string name, std::function<void()> body, dhe::unit::Context *context);

    void run();

  private:
    std::string const name;
    std::function<void()> const body;
    dhe::unit::Context *context;
  };

} // namespace unit
} // namespace dhe
