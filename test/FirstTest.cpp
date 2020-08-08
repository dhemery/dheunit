#include "Test.h"

using dhe::unit::Test;
using dhe::unit::Reporter;

class MyTest : public Test {
public:
  MyTest() : Test{"my!!test"} {}
  void run(Reporter &r) override {
    r.fatal("hey a fatal thing");
    r.error("hey an error thing");
  }
};

static MyTest t __attribute__((unused)){};