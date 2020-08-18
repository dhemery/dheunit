#include "Latch.h"

#include "dheunit/test.h"

using dhe::Latch;
using dhe::unit::AddTestFunc;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

namespace dhe {
namespace components {
  static auto constexpr low = Latch{false, false};
  static auto constexpr falling = Latch{false, true};
  static auto constexpr rising = Latch{true, true};
  static auto constexpr high = Latch{true, false};

  static auto checkEquality(const Latch &a, const Latch &b, bool wantEQ) -> TestFunc;
  static auto checkClock(Latch l, bool signal, Latch wantLatch) -> TestFunc;

  class LatchSuite : public Suite {
  public:
    LatchSuite() : Suite{"dhe::Latch"} {}

    void addTests(AddTestFunc addTest) override {
      addTest("is low by default", checkEquality(Latch{}, low, true));

      addTest("high == high", checkEquality(high, high, true));
      addTest("high != low", checkEquality(high, low, false));
      addTest("high != rising", checkEquality(high, rising, false));
      addTest("high != falling", checkEquality(high, falling, false));
      addTest("low != high", checkEquality(low, high, false));
      addTest("low == low", checkEquality(low, low, true));
      addTest("low != rising", checkEquality(low, rising, false));
      addTest("low != falling", checkEquality(low, falling, false));
      addTest("rising != high", checkEquality(rising, high, false));
      addTest("rising != low", checkEquality(rising, low, false));
      addTest("rising == rising", checkEquality(rising, rising, true));
      addTest("rising != falling", checkEquality(rising, falling, false));
      addTest("falling != high", checkEquality(falling, high, false));
      addTest("falling != low", checkEquality(falling, low, false));
      addTest("falling != rising", checkEquality(falling, rising, false));
      addTest("falling == falling", checkEquality(falling, falling, true));

      addTest("high + high signal → unchanged", checkClock(high, true, high));
      addTest("high + low signal → falls", checkClock(high, false, falling));

      addTest("low + high signal → rises", checkClock(low, true, rising));
      addTest("low + low signal → unchanged", checkClock(low, false, low));

      addTest("rising + high signal → loses edge", checkClock(rising, true, high));
      addTest("rising + low signal → falling", checkClock(rising, false, falling));

      addTest("falling + high signal → rises", checkClock(falling, true, rising));
      addTest("falling + low signal → loses edge", checkClock(falling, false, low));
    }
  };

  static LatchSuite latchSuite __attribute__((unused)){};

  auto checkClock(Latch l, bool signal, Latch wantLatch) -> TestFunc {
    return [l, signal, wantLatch](Tester &t) mutable {
      l.clock(signal);

      if (l != wantLatch) {
        t.errorf("Got {}, want {}", l, wantLatch);
      }
    };
  }

  auto checkEquality(const Latch &a, const Latch &b, bool wantEQ) -> TestFunc {
    return [a, b, wantEQ](Tester &t) {
      auto const aeqb = (a == b);
      if (aeqb != wantEQ) {
        t.errorf("{}=={} got {}, want {}", a, b, aeqb, !aeqb);
      }
      auto const beqa = (b == a);
      if (beqa != wantEQ) {
        t.errorf("{}=={} got {}, want {}", b, a, beqa, !beqa);
      }
      auto const aneb = (a != b);
      if (aneb == wantEQ) {
        t.errorf("{}!={} got {}, want {}", a, b, aneb, !aneb);
      }
      auto const bnea = (b != a);
      if (bnea == wantEQ) {
        t.errorf("{}!={} got {}, want {}", b, a, bnea, !bnea);
      }
    };
  }

} // namespace components
} // namespace dhe
