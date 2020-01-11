#include <string>
#include <vector>
#include <iostream>

namespace dhe {

namespace test {

class Context {
public:
  Context(std::string name, Context *parent) : name{name}, parent{parent} {}

  void addBefore(std::function<void()> before) {
    befores.push_back(before);
  }

  void addAfter(std::function<void()> after) {
    afters.push_back(after);
  }

  void create() {
    if (parent != nullptr) {
      parent->create();
    }
    for (auto before : befores) {
      before();
    }
  }

  void destroy() {
    for (auto after = afters.rbegin(); after != afters.rend(); after++) {
      (*after)();
    }
    if (parent != nullptr) {
      parent->destroy();
    }
  }

private:
  std::string const name;
  Context *parent;
  std::vector<std::function<void()>> befores;
  std::vector<std::function<void()>> afters;
};

class Test {
public:
  Test(std::string name, std::function<void()> body, Context *context) : name{name}, body{body}, context{context} {}

  void run() {
    context->create();
    body();
    context->destroy();
  }

private:
  std::string const name;
  std::function<void()> const body;
  Context *context;
};

class Suite {
public:
  Suite() noexcept {};

  void addContext(std::string const name, std::function<void()> body) {
    auto parent = context;
    context = new Context{name, parent};
    body();
    context = parent;
  }

  void addBefore(std::function<void()> body) {
    context->addBefore(body);
  }

  void addTest(std::string const name, std::function<void()> body) {
    tests.emplace_back(name, body, context);
  }

  void addAfter(std::function<void()> body) {
    context->addAfter(body);
  }

  void runTests() {
    for (auto test : tests) {
      test.run();
    }
  }

  static auto suite() -> Suite & {
    static auto theSuite = Suite{};
    return theSuite;
  }

  static void run() {
    suite().runTests();
  }

private:
  Context root{"root", nullptr};
  std::vector<Test> tests{};
  Context *context{&root};
};

auto describe(std::string const name, std::function<void()> body) -> bool {
  Suite::suite().addContext(name, body);
  return true;
}

void before(std::function<void()> body) {
  Suite::suite().addBefore(body);
}

void it(std::string const name, std::function<void()> body) {
  Suite::suite().addTest(name, body);
}

void after(std::function<void()> body) {
  Suite::suite().addAfter(body);
}
}
}

using dhe::test::describe;
using dhe::test::before;
using dhe::test::it;
using dhe::test::after;

static auto _ = describe("OUTER", []() {
  before([]() {
    std::cout << "OUTER before" << std::endl;
  });
  after([]() {
    std::cout << "OUTER after" << std::endl;

  });

  describe("INNER", [&]() {
    before([]() {
      std::cout << "INNER before" << std::endl;
    });

    after([]() {
      std::cout << "INNER after" << std::endl;

    });

    it("IT1", [&]() {
      std::cout << "IT1" << std::endl;
      throw 30;
    });

    it("IT2", [&]() {
      std::cout << "IT2" << std::endl;
    });

    it("IT3", [&]() {
      std::cout << "IT3" << std::endl;
    });
  });
});

int main() {
  dhe::test::Suite::run();
}