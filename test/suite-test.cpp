#include "dheunit.h"
#include "suite.h"

using dhe::unit::describe;
using dhe::unit::it;
using dhe::unit::Suite;

static auto _ = describe("something about the suite", []() {
  auto suite = Suite{};
  it("does something fun", []() {});
});