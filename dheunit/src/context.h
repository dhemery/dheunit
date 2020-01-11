#pragma once

#include <functional>
#include <string>
#include <vector>

namespace dhe {
namespace unit {
  class Context {
  public:
    Context(std::string name, Context *parent);

    void addBefore(std::function<void()> const &before);

    void addAfter(std::function<void()> const &after);

    void create();

    void destroy();

  private:
    std::string const name;
    Context *parent;
    std::vector<std::function<void()>> befores;
    std::vector<std::function<void()>> afters;
  };

} // namespace unit
} // namespace dhe
