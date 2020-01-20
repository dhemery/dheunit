#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dhe {
namespace unit {
  class Context {
  public:
    Context(std::string name, std::shared_ptr<Context> parent);

    void addBefore(std::function<void()> const &before);

    void addAfter(std::function<void()> const &after);

    void setUp();

    void tearDown();

    auto description() const -> std::string;

  private:
    std::string const name;
    std::shared_ptr<Context> const parent;
    std::vector<std::function<void()>> befores;
    std::vector<std::function<void()>> afters;
  };

} // namespace unit
} // namespace dhe
