#include "suite.h"

#include "snowhouse/assertionexception.h"
#include "test.h"

#include <iostream>
#include <sstream>

using snowhouse::AssertionException;

namespace dhe {
namespace unit {
  static void reportFailure(std::shared_ptr<Test> const &test, AssertionException const &failure) {
    auto message = std::ostringstream{};
    message << "FAILURE: " << test->description() << std::endl;
    message << failure.GetFilename() << ":" << failure.GetLineNumber() << std::endl;
    message << failure.GetMessage();
    std::cout << message.str();
  }

  static void reportSuccess(std::shared_ptr<Test> const &test) {
    std::cout << "SUCCESS: " << test->description() << std::endl;
  }

  void Suite::addTest(std::shared_ptr<Test> const &test) { tests.push_back(test); }

  void Suite::run() {
    for (const auto &test : tests) {
      try {
        test->run();
        reportSuccess(test);
      } catch (AssertionException const &failure) {
        reportFailure(test, failure);
      }
    }
  }

  auto suite() -> Suite & {
    static auto theSuite = Suite{};
    return theSuite;
  }
} // namespace unit
} // namespace dhe
