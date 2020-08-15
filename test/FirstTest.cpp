#include "dheunit.h"

using dhe::unit::Logger;
using dhe::unit::Test;

class FailingTest : public Test {
public:
  FailingTest() : Test{"FailingTest"} {}
  void run(Logger &l) override {
    auto constexpr someInt{9};
    auto constexpr someBool{true};
    l.log("hey a log thing", someInt, someBool);
    l.error("hey an error thing");
    l.fatal("hey a fatal thing");
  }
};

static FailingTest failingTest __attribute__((unused)){};

class PassingTest : public Test {
public:
  PassingTest() : Test{"PassingTest"} {}
  void run(Logger &l) override {
    l.log("hey a log thing");
    l.log("hey another log thing");
  }
};
static PassingTest passingTest __attribute__((unused)){};

class ThrowingTest : public Test {
public:
  ThrowingTest() : Test{"ThrowingTest"} {}
  void run(Logger & /**/) override { throw std::invalid_argument{"thrown by test"}; }
};
static ThrowingTest throwingTest __attribute__((unused)){};

class StringThrowingTest : public Test {
public:
  StringThrowingTest() : Test{"StringThrowingTest"} {}
  void run(Logger & /**/) override { throw "string thrown by test"; }
};

static StringThrowingTest stringThrowingTest __attribute__((unused)){};

class FormattingTest : public Test {
public:
  FormattingTest() : Test{"FormattingTest"} {}
  void run(Logger &l) override {
    l.logf("{} → {}", true, false);
    l.errorf("{} →→→→ {} extra stuff beyond the parameters", false, "error monkey");
    l.fatalf("{} →→→→ {}", 3, "fatal monkey");
  }
};

static FormattingTest formattingTest __attribute__((unused)){};
