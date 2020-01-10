#include <string>
#include <vector>
#include <iostream>

namespace dhe {

namespace test {

static void describe(std::string const &name, std::function<void()> body) {
  std::cout << "Added description " << name << std::endl;
  body();
  std::cout << "Done description " << name << std::endl;
}

static void it(std::string const &name, std::function<void()> body) {
  std::cout << "Adding test " << name << std::endl;
  body();
  std::cout << "Done test " << name << std::endl;
}
};
}

using dhe::test::describe;
using dhe::test::it;

int main() {
  describe("OUTER", [&]() {
    describe("INNER", [&]() {
      it("IT", [&]() {
      });
    });
  });
}
