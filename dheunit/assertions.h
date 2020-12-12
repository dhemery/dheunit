#pragma once

#include "test.h"
#include <functional>

namespace dhe {
namespace unit {

template <typename Actual>
auto is_equal_to(Actual const &want)
    -> std::function<void(Tester &, Actual const &)> {
  return [want](Tester &t, Actual const &actual) {
    if (actual != want) {
      t.errorf("was {}, want a value equal to {}", actual, want);
    }
  };
}

template <typename Actual>
auto is_not_equal_to(Actual const &want)
    -> std::function<void(Tester &, Actual const &)> {
  return [want](Tester &t, Actual const &actual) {
    if (actual == want) {
      t.errorf("was {}, want a value not equal to {}", actual, want);
    }
  };
}

template <typename Actual>
auto is_near(Actual const &want, Actual const &tolerance)
    -> std::function<void(Tester &, Actual const &)> {
  return [want, tolerance](Tester &t, Actual const &actual) {
    auto const min = want - tolerance;
    auto const max = want + tolerance;
    if (actual < min || actual > max) {
      t.errorf("was {}, want within {} of {}", actual, tolerance, want);
    }
  };
}

template <typename Actual>
auto is_greater_than(Actual const &max)
    -> std::function<void(Tester &, Actual const &)> {
  return [max](Tester &t, Actual const &actual) {
    if (!(actual > max)) {
      t.errorf("was {}, want greater than {}", actual, max);
    }
  };
}

template <typename Actual>
auto is_no_greater_than(Actual const &max)
    -> std::function<void(Tester &, Actual const &)> {
  return [max](Tester &t, Actual const &actual) {
    if (actual > max) {
      t.errorf("was {}, want no greater than {}", actual, max);
    }
  };
}

template <typename Actual>
auto is_between(Actual const &min, Actual const &max)
    -> std::function<void(Tester &, Actual const &)> {
  return [min, max](Tester &t, Actual const &actual) {
    if (actual < min || actual > max) {
      t.errorf("was {}, want between {} and {}", actual, min, max);
    }
  };
}

template <typename Actual>
auto is_less_than(Actual const &min)
    -> std::function<void(Tester &, Actual const &)> {
  return [min](Tester &t, Actual const &actual) {
    if (!(actual < min)) {
      t.errorf("was {}, want less than {}", actual, min);
    }
  };
}

template <typename Actual>
auto is_no_less_than(Actual const &min)
    -> std::function<void(Tester &, Actual const &)> {
  return [min](Tester &t, Actual const &actual) {
    if (actual < min) {
      t.errorf("was {}, want no less than {}", actual, min);
    }
  };
}

static inline void is_true(Tester &t, bool actual) {
  if (!actual) {
    t.error("was false, want true");
  }
}

static inline void is_false(Tester &t, bool actual) {
  if (actual) {
    t.error("was true, want false");
  }
}
} // namespace unit
} // namespace dhe
