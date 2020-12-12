#pragma once

#include "test.h"
#include <functional>

namespace dhe {
namespace unit {

template <typename T>
using Assertion = std::function<void(Tester &, T const &)>;

template <typename T> auto is_equal_to(T const &want) -> Assertion<T> {
  return [want](Tester &t, T const &actual) {
    if (actual != want) {
      t.errorf("was {}, want a value equal to {}", actual, want);
    }
  };
}

template <typename T> auto is_not_equal_to(T const &want) -> Assertion<T> {
  return [want](Tester &t, T const &actual) {
    if (actual == want) {
      t.errorf("was {}, want a value not equal to {}", actual, want);
    }
  };
}

template <typename T>
auto is_near(T const &want, T const &tolerance) -> Assertion<T> {
  return [want, tolerance](Tester &t, T const &actual) {
    auto const min = want - tolerance;
    auto const max = want + tolerance;
    if (actual < min || actual > max) {
      t.errorf("was {}, want within {} of {}", actual, tolerance, want);
    }
  };
}

template <typename T> auto is_greater_than(T const &max) -> Assertion<T> {
  return [max](Tester &t, T const &actual) {
    if (!(actual > max)) {
      t.errorf("was {}, want greater than {}", actual, max);
    }
  };
}

template <typename T> auto is_no_greater_than(T const &max) -> Assertion<T> {
  return [max](Tester &t, T const &actual) {
    if (actual > max) {
      t.errorf("was {}, want no greater than {}", actual, max);
    }
  };
}

template <typename T>
auto is_between(T const &min, T const &max) -> Assertion<T> {
  return [min, max](Tester &t, T const &actual) {
    if (actual < min || actual > max) {
      t.errorf("was {}, want between {} and {}", actual, min, max);
    }
  };
}

template <typename T> auto is_less_than(T const &min) -> Assertion<T> {
  return [min](Tester &t, T const &actual) {
    if (!(actual < min)) {
      t.errorf("was {}, want less than {}", actual, min);
    }
  };
}

template <typename T> auto is_no_less_than(T const &min) -> Assertion<T> {
  return [min](Tester &t, T const &actual) {
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
