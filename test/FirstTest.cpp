#include "dheunit.h"

using dhe::unit::Reporter;
using dhe::unit::Test;

class FailingTest : public Test {
public:
  FailingTest() : Test{"FailingTest"} {}
  void run(Reporter *r) override {
    r->fatal() << "hey a fatal thing";
    r->error() << "hey an error thing";
    r->error() << "hey a log thing";
  }
};

class PassingTest : public Test {
public:
  PassingTest() : Test{"PassingTest"} {}
  void run(Reporter *r) override {
    r->log() << "hey a log thing";
    r->log() << "hey another log thing";
  }
};

class ThrowingTest : public Test {
public:
  ThrowingTest() : Test{"ThrowingTest"} {}
  void run(Reporter * /**/) override { throw std::invalid_argument{"thrown by throwing test"}; }
};

static FailingTest failingTest __attribute__((unused)){};
static PassingTest passingTest __attribute__((unused)){};
static ThrowingTest throwingTest __attribute__((unused)){};
