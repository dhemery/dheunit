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
    auto test(std::function<void(Reporter &)> const &test) -> Suite&;

    void run();

  private:
    std::vector<std::function<void(Reporter &)>> tests{};
  };

  auto suite() -> Suite &;
} // namespace unit
} // namespace dhe
