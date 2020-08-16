#include "Latch.h"

#include "dheunit.h"

using dhe::Latch;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;
using dhe::unit::TestMap;

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

    void addTests(TestMap &tests) override {
      tests["is low by default"] = checkEquality(Latch{}, low, true);

      tests["high == high"] = checkEquality(high, high, true);
      tests["high != low"] = checkEquality(high, low, false);
      tests["high != rising"] = checkEquality(high, rising, false);
      tests["high != falling"] = checkEquality(high, falling, false);
      tests["low != high"] = checkEquality(low, high, false);
      tests["low == low"] = checkEquality(low, low, true);
      tests["low != rising"] = checkEquality(low, rising, false);
      tests["low != falling"] = checkEquality(low, falling, false);
      tests["rising != high"] = checkEquality(rising, high, false);
      tests["rising != low"] = checkEquality(rising, low, false);
      tests["rising == rising"] = checkEquality(rising, rising, true);
      tests["rising != falling"] = checkEquality(rising, falling, false);
      tests["falling != high"] = checkEquality(falling, high, false);
      tests["falling != low"] = checkEquality(falling, low, false);
      tests["falling != rising"] = checkEquality(falling, rising, false);
      tests["falling == falling"] = checkEquality(falling, falling, true);

      tests["high + high signal → unchanged"] = checkClock(high, true, high);
      tests["high + low signal → falls"] = checkClock(high, false, falling);

      tests["low + high signal → rises"] = checkClock(low, true, rising);
      tests["low + low signal → unchanged"] = checkClock(low, false, low);

      tests["rising + high signal → loses edge"] = checkClock(rising, true, high);
      tests["rising + low signal → falling"] = checkClock(rising, false, falling);

      tests["falling + high signal → rises"] = checkClock(falling, true, rising);
      tests["falling + low signal → loses edge"] = checkClock(falling, false, low);
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
