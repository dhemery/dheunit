#pragma once
#include "test.h"

#include <memory>

namespace dhe {
namespace unit {
  class Suite {
  public:
    void addTest(std::shared_ptr<Test> const &test);

    void run();

  private:
    std::vector<std::shared_ptr<Test>> tests{};
  }; // namespace unit

  auto suite() -> Suite &;
} // namespace unit
} // namespace dhe
