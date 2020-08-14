#include "dheunit.h"

using dhe::unit::Logger;
using dhe::unit::Test;

class FailingTest : public Test {
public:
  FailingTest() : Test{"FailingTest"} {}
  void run(Logger &l) override {
    auto constexpr someInt{9};
    auto constexpr someBool{true};
    l.log("hey a log thing ", someInt, " ", someBool);
    l.error("hey an error thing");
    l.fatal("hey a fatal thing");
  }
};

class PassingTest : public Test {
public:
  PassingTest() : Test{"PassingTest"} {}
  void run(Logger &l) override {
    l.log("hey a log thing");
    l.log("hey another log thing");
  }
};

class ThrowingTest : public Test {
public:
  ThrowingTest() : Test{"ThrowingTest"} {}
  void run(Logger & /**/) override { throw std::invalid_argument{"thrown by test"}; }
};

class StringThrowingTest : public Test {
public:
  StringThrowingTest() : Test{"StringThrowingTest"} {}
  void run(Logger & /**/) override { throw "string thrown by test"; }
};

static FailingTest failingTest __attribute__((unused)){};
static PassingTest passingTest __attribute__((unused)){};
static ThrowingTest throwingTest __attribute__((unused)){};
static StringThrowingTest stringThrowingTest __attribute__((unused)){};
