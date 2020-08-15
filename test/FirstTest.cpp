#include "dheunit.h"

using dhe::unit::Test;
using dhe::unit::Tester;

class FailingTest : public Test {
public:
  FailingTest() : Test{"FailingTest"} {}
  void run(Tester &t) override {
    auto constexpr someInt{9};
    auto constexpr someBool{true};
    t.log("hey a log thing", someInt, someBool);
    t.error("hey an error thing");
    t.fatal("hey a fatal thing");
  }
};

static FailingTest failingTest __attribute__((unused)){};

class PassingTest : public Test {
public:
  PassingTest() : Test{"PassingTest"} {}
  void run(Tester &t) override {
    t.log("hey a log thing");
    t.log("hey another log thing");
  }
};
static PassingTest passingTest __attribute__((unused)){};

class ThrowingTest : public Test {
public:
  ThrowingTest() : Test{"ThrowingTest"} {}
  void run(Tester & /**/) override { throw std::invalid_argument{"thrown by test"}; }
};
static ThrowingTest throwingTest __attribute__((unused)){};

class StringThrowingTest : public Test {
public:
  StringThrowingTest() : Test{"StringThrowingTest"} {}
  void run(Tester & /**/) override { throw "string thrown by test"; }
};

static StringThrowingTest stringThrowingTest __attribute__((unused)){};

class FormattingTest : public Test {
public:
  FormattingTest() : Test{"FormattingTest"} {}
  void run(Tester &l) override {
    l.logf("{} → {}", true, false);
    l.errorf("{} →→→→ {} extra stuff beyond the parameters", false, "error monkey");
    l.fatalf("{} →→→→ {}", 3, "fatal monkey");
  }
};

static FormattingTest formattingTest __attribute__((unused)){};
