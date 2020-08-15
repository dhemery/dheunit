#pragma once

#include <iostream>
#include <string>

namespace dhe {
class Latch {
public:
  constexpr Latch() = default;
  constexpr Latch(bool state, bool edge) : state{state}, edge{edge} {}
  //  Latch(Latch const &) = default;
  //  auto operator=(Latch const &) -> Latch& = default;
  void clock(bool signal) {
    edge = signal != state;
    state = signal;
  }

  auto isHigh() const -> bool { return state; };
  auto isEdge() const -> bool { return edge; };

  auto isLow() const -> bool { return !isHigh(); };
  auto isFall() const -> bool { return isEdge() && isLow(); };
  auto isRise() const -> bool { return isEdge() && isHigh(); };

  friend auto operator==(Latch const &left, Latch const &right) -> bool {
    return left.state == right.state && left.edge == right.edge;
  }

  friend auto operator<<(std::ostream &o, Latch const &l) -> std::ostream & {
    return o << "Latch{" << l.isHigh() << ',' << l.isEdge() << '}';
  }

private:
  bool state{};
  bool edge{};
};

} // namespace dhe
