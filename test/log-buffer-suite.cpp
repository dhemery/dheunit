#include "dheunit/internal/buffered-log.h"

#include "dheunit/test.h"

#include <algorithm>
#include <array>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>

namespace dhe {
namespace unit {
namespace log {
namespace test {
enum class Op { Begin, Write, End };

static auto op_names = std::array<std::string, 3>{"Begin", "Write", "End"};

struct Entry {
  Op op_;
  std::string line_;

  auto operator==(Entry const &r) const -> bool {
    return op_ == r.op_ && line_ == r.line_;
  }

  friend std::ostream &operator<<(std::ostream &o, Entry e) {
    o << op_names[static_cast<int>(e.op_)] << ": " << e.line_;
    return o;
  }
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
  if (!v.empty()) {
    out << '[';
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

struct SpyLog : public Log {
  void begin(std::string const &name) override {
    ops_.push_back({Op::Begin, name});
  }

  void write(std::string const &line) override {
    ops_.push_back({Op::Write, line});
  }

  void end() override { ops_.push_back({Op::End, ""}); };

  std::vector<Entry> ops_{};
};

class LogBufferSuite : Suite {
public:
  LogBufferSuite() : Suite("LogBuffer") {}

  void run(Tester &t) override {
    t.run("write(line) writes to the log", [](Tester &t) {
      auto const section_name = std::string("section-name");
      auto log = SpyLog{};
      auto buf = LogBuffer{section_name, &log};

      buf.write("a line to record");

      auto want = std::vector<Entry>{
          {Op::Write, "a line to record"},
      };

      if (!std::equal(log.ops_.begin(), log.ops_.end(), want.begin())) {
        t.errorf("Got ops {}, want {}", log.ops_, want);
      }
    });
  }
};

static auto _ = LogBufferSuite{};
} // namespace test
} // namespace log
} // namespace unit
} // namespace dhe
