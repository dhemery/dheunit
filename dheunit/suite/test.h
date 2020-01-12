#pragma once

#include "context.h"

#include <functional>
#include <memory>
#include <string>

namespace dhe {
namespace unit {
  class Test {
  public:
    Test(std::string name, std::function<void()> body, std::shared_ptr<Context> context);

    void run();

  private:
    std::string const name;
    std::function<void()> const body;
    std::shared_ptr<Context> context;
  };
} // namespace unit
} // namespace dhe
