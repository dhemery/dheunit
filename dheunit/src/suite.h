#pragma once
#include "context.h"
#include "test.h"

#include <functional>
#include <string>

namespace dhe {
namespace unit {
  class Suite {
  public:
    void addContext(std::string const &name, std::function<void()> const &body);

    void addBefore(std::function<void()> const &body);

    void addTest(std::string const &name, const std::function<void()> &body);

    void addAfter(std::function<void()> const &body);

    void runTests();

  private:
    std::vector<Context> contexts{{"suite", nullptr}};
    std::vector<Test const> tests{};
    Context *context{&contexts[0]};
  }; // namespace unit

} // namespace unit
} // namespace dhe
