#pragma once

#include <functional>
#include <string>
#include <vector>

namespace dhe {
namespace unit {
  class Reporter {
    void error(std::string);
    void fatal(std::string);
  };

  class Suite {
  public:
    void test(std::function<void(Reporter &)> const &test);

    void run();

  private:
    std::vector<std::function<void(Reporter &)>> tests{};
  };

  auto suite() -> Suite &;
} // namespace unit
} // namespace dhe
