#include "Runner.h"
#include "dheunit.h"

#include <iostream>
#include <utility>
#include <vector>

namespace dhe {
namespace unit {
  class Suite {
  public:
    void add(Test *test) { tests.push_back(test); }

    auto run() -> int {
      for (auto *test : tests) {
        auto runner = Runner{};
        runner.run(test);
        auto const &result = runner.failed() ? "FAILED " : "PASSED ";
        std::cout << result << test->name() << std::endl;
        for (auto const &log : runner.logs()) {
          std::cout << "    " << log << std::endl;
        }
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

  Test::Test(std::string name) : testName{std::move(name)} { suite().add(this); }

  auto Test::runAll() -> int { return suite().run(); }
} // namespace unit
} // namespace dhe
