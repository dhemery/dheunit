#pragma once

#include "dheunit.h"

#include <sstream>
#include <string>
#include <vector>

namespace dhe {
namespace unit {
  class Runner : public virtual Reporter {
  public:
    auto log() -> std::ostream & override;
    auto error() -> std::ostream & override;
    auto fatal() -> std::ostream & override;
    void fail() override;
    void failNow() override;

    void run(Test *test);
    auto failed() const -> bool;
    auto logs() const -> std::vector<std::string>;

  private:
    bool testFailed{false};
    std::vector<std::ostringstream> testLogs{};
  };
} // namespace unit
} // namespace dhe
