#pragma once

#include <iostream>
#include <sstream>
#include <string>

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

  friend auto operator<<(std::ostream &o, Latch const &l) -> std::ostream & {
    return o << "Latch{" << l.isHigh() << ',' << l.isEdge() << '}';
  }

  auto str() const -> std::string { return (std::ostringstream{} << std::boolalpha << *this).str(); }
  auto operator==(Latch const &rhs) const -> bool { return state == rhs.state && edge == rhs.edge; }
  auto operator!=(Latch const &rhs) const -> bool { return !(rhs == *this); }

private:
  bool state{};
  bool edge{};
};

} // namespace dhe
