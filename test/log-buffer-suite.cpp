#include "dheunit/internal/log-buffer.h"

#include "dheunit/test.h"

#include <algorithm>
#include <string>
#include <vector>

namespace dhe {
namespace unit {
namespace log {
namespace test {

class SpyLog : public Log {
public:
  void begin(std::string const &name) override { begins_.push_back(name); }

  void write(std::string const &line) override { writes_.push_back(line); }

  void end(bool always) override { ends_.push_back(always); }

  std::vector<std::string> begins_{}; // NOLINT
  std::vector<std::string> writes_{}; // NOLINT
  std::vector<bool> ends_{};          // NOLINT
};

class LogBufferSuite : Suite {
public:
  LogBufferSuite() : Suite("LogBuffer") {}

  void run(Tester &t) override {
    t.run("first announce() calls log.begin(name)", [](Tester &t) {
      auto const name = std::string("log-buffer-name");
      auto log = SpyLog{};
      auto buf = LogBuffer{name, &log};

      buf.announce();

      auto const n_written = log.begins_.size();
      if (n_written != 1) {
        t.fatalf("Got {} lines, expected 1", n_written);
      }
      auto const line_written = log.begins_.front();
      if (line_written != name) {
        t.errorf("Got line '{}', want '{}'", line_written, name);
      }
    });

    t.run("subsequent announce() does not call log.begin(name)", [](Tester &t) {
      auto const name = std::string("log-buffer-name");
      auto log = SpyLog{};
      auto buf = LogBuffer{name, &log};

      buf.announce();
      log.begins_.clear();

      buf.announce();
      buf.announce();
      buf.announce();
      buf.announce();

      if (!log.begins_.empty()) {
        t.errorf("Want no output, got at least '{}'", log.begins_.front());
      }
    });

    t.run("flush() writes buffered lines to log", [](Tester &t) {
      auto const lines = std::vector<std::string>{
          "buffered line 1",
          "buffered line 2",
          "buffered line 3",
          "buffered line 4",
      };
      auto log = SpyLog{};
      auto buf = LogBuffer{"", &log};

      for (auto const &line : lines) {
        buf.write(line);
      }

      if (!log.writes_.empty()) {
        t.errorf("Wrote {} lines before flush()", log.writes_.size());
      }

      buf.flush();

      if (log.writes_.size() != lines.size()) {
        t.fatal("Got {} lines, want {}", log.writes_.size(), lines.size());
      }
      auto const result =
          std::mismatch(lines.cbegin(), lines.cend(), log.writes_.cbegin());
      if (result.first != lines.cend()) {
        t.errorf(R"(Mismatch at {}, want "{}", got "{}")",
                 result.first - lines.cbegin(), *result.first, *result.second);
      }

      if (log.ends_.size() != 1) {
        t.errorf("log.end() called {} times, want {}", log.ends_.size(), 1);
      }
    });
  }
};

LogBufferSuite __attribute__((unused)) _{};
} // namespace test
} // namespace log
} // namespace unit
} // namespace dhe
