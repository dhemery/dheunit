#include "dheunit.h"

#include <iostream>

using dhe::unit::after;
using dhe::unit::before;
using dhe::unit::describe;
using dhe::unit::it;

static auto _ = describe("OUTER", []() {
  before([]() { std::cout << "OUTER before" << std::endl; });
  after([]() { std::cout << "OUTER after" << std::endl; });

  describe("INNER", [&]() {
    before([]() { std::cout << "INNER before" << std::endl; });

    after([]() { std::cout << "INNER after" << std::endl; });

    it("IT1", [&]() { std::cout << "IT1" << std::endl; });

    it("IT2", [&]() { std::cout << "IT2" << std::endl; });

    it("IT3", [&]() { std::cout << "IT3" << std::endl; });
  });
});
