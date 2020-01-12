#include "dheunit.h"
#include "snowhouse/snowhouse.h"

using dhe::unit::after;
using dhe::unit::before;
using dhe::unit::describe;
using dhe::unit::it;
using snowhouse::Is;

static auto _ = describe("OUTER", []() {
  before([]() {});
  after([]() {});

  describe("INNER", [&]() {
    before([]() {});

    after([]() {});

    it("IT1", [&]() { AssertThat(12, Is().LessThan(11).And().GreaterThan(99)); });

    it("IT2", [&]() {});

    it("IT3", [&]() {});
  });
});
