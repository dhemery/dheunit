#include <functional>
#include <string>

namespace dhe {

namespace unit {
  void after(std::function<void()> const &body);
  void before(std::function<void()> const &body);
  auto describe(const std::string &name, std::function<void()> const &body) -> bool;
  void it(const std::string &name, std::function<void()> const &body);
} // namespace unit
} // namespace dhe
