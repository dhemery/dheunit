#include "test.h"

#include "dheunit.h"

#include <memory>

using dhe::unit::Context;
using dhe::unit::describe;
using dhe::unit::it;
using dhe::unit::Test;

static auto _ __attribute__((unused)) = //
    describe("something about the test", []() {
      auto noContext = std::shared_ptr<Context>{};
      auto context = std::make_shared<Context>("fun-context", noContext);
      auto test = Test{"test-name", []() {}, context};

      it("does something fun", []() {});
    });
