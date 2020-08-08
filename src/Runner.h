#pragma once

#include "dheunit.h"

#include <string>
#include <vector>

namespace dhe {
namespace unit {
  class Runner : public virtual Reporter {
  public:
    void log(const std::string &msg) override;
    void error(const std::string &msg) override;
    void fatal(const std::string &msg) override;
    void fail() override;
    void failNow() override;

    void run(Test *test);
    auto failed() const -> bool;
    auto logs() const -> std::vector<std::string>;

  private:
    bool testFailed{false};
    std::vector<std::string> testLogs{};
  };
} // namespace unit
} // namespace dhe
