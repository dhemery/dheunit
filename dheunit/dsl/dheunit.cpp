#include "dheunit.h"

#include "context.h"
#include "suite.h"
#include "test.h"

namespace dhe {
namespace unit {
  static auto contextStack() -> std::vector<std::shared_ptr<Context>> & {
    static std::vector<std::shared_ptr<Context>> contextStack{};
    return contextStack;
  };

  static auto context() -> std::shared_ptr<Context> {
    static const auto noContext = std::shared_ptr<Context>{};
    return contextStack().empty() ? noContext : contextStack().back();
  }

  void after(const std::function<void()> &body) { context()->addAfter(body); }

  void before(std::function<void()> const &body) { context()->addBefore(body); }

  auto describe(const std::string &name, std::function<void()> const &builder) -> bool {
    contextStack().push_back(std::make_shared<Context>(name, context()));
    builder();
    contextStack().pop_back();
    return true;
  }

  void it(const std::string &name, std::function<void()> const &body) {
    suite().addTest(std::make_shared<Test>(name, body, context()));
  }
} // namespace unit
} // namespace dhe
