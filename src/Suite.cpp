#include "Runner.h"
#include "dheunit.h"

#include <utility>
#include <vector>

namespace dhe {
namespace unit {
  class Suite {
  public:
    void add(Test *test) { tests.push_back(test); }

    auto run() -> int {
      for (const auto &test : tests) {
        auto runner = Runner{};
        runner.run(test);
      }
      return 0;
    }

  private:
    std::vector<Test *> tests{};
  };

  static auto suite() -> Suite & {
    static auto theSuite = Suite{};
    return theSuite;
  }

  Test::Test(std::string name) : name{std::move(name)} { suite().add(this); }

  auto Test::runAll() -> int { return suite().run(); }
} // namespace unit
} // namespace dhe
