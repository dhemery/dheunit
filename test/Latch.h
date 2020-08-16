#pragma once

#include <iostream>

namespace dhe {
class Latch {
public:
  constexpr Latch() = default;
  constexpr Latch(bool state, bool edge) : state{state}, edge{edge} {}
  void clock(bool signal) {
    edge = signal != state;
    state = signal;
  }

  auto isHigh() const -> bool { return state; };
  auto isEdge() const -> bool { return edge; };

  auto isLow() const -> bool { return !isHigh(); };
  auto isFall() const -> bool { return isEdge() && isLow(); };
  auto isRise() const -> bool { return isEdge() && isHigh(); };

  auto operator==(Latch const &rhs) const -> bool { return state == rhs.state && edge == rhs.edge; }
  auto operator!=(Latch const &rhs) const -> bool { return !(rhs == *this); }

  friend auto operator<<(std::ostream &os, Latch const &latch) -> std::ostream & {
    os << "Latch{" << latch.state << ',' << latch.edge << '}';
    return os;
  }

private:
  bool state{};
  bool edge{};
};

} // namespace dhe
