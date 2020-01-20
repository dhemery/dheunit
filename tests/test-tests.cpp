#include "dheunit.h"
#include "snowhouse/snowhouse.h"
#include "test.h"

#include <memory>

using dhe::unit::Context;
using dhe::unit::describe;
using dhe::unit::it;
using dhe::unit::Test;
using snowhouse::Is;

static inline auto createContext() -> std::shared_ptr<Context> {
  auto noContext = std::shared_ptr<Context>{};
  return std::make_shared<Context>("some context", noContext);
}

static auto _ __attribute__((unused)) = //
    describe("dhe::unit::Test", []() {

      describe("run()", []() {

        it("runs the test body", []() {
          auto context = createContext();
          auto ranTestBody = false;

          auto testBody = [&]() { ranTestBody = true; };

          auto test = Test{"some test", testBody, context};

          test.run();

          AssertThat(ranTestBody, Is().True());
        });

        it("sets up the context before running the test body", []() {
          auto setUpContext = false;
          auto setUpContextBeforeRunningTest = false;

          auto context = createContext();
          context->addBefore([&]() { setUpContext = true; });

          auto testBody = [&]() { setUpContextBeforeRunningTest = setUpContext; };

          auto test = Test{"some test", testBody, context};

          test.run();

          AssertThat(setUpContextBeforeRunningTest, Is().True());
        });

        it("tears down the context after running the test body", []() {
          auto ranTest = false;
          auto toreDownContextAfterRunningTest = false;

          auto testBody = [&]() { ranTest = true; };

          auto context = createContext();
          context->addAfter([&]() { toreDownContextAfterRunningTest = ranTest; });

          auto test = Test{"some test", testBody, context};

          test.run();

          AssertThat(toreDownContextAfterRunningTest, Is().True());
        });
      });
    });
