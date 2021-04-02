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
    t.run("announce() begins named log section if not already begun",
          [](Tester &t) {
            auto const name = std::string("section-name");
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

    t.run("announce() does not begin log section if already begun",
          [](Tester &t) {
            auto const name = std::string("section-name");
            auto log = SpyLog{};
            auto buf = LogBuffer{name, &log};

            buf.announce();
            log.begins_.clear();

            buf.announce();
            buf.announce();
            buf.announce();
            buf.announce();

            if (!log.begins_.empty()) {
              t.errorf("Want no output, got at least '{}'",
                       log.begins_.front());
            }
          });

    t.run("write() begins named log section if not already begun",
          [](Tester &t) {
            auto const section_name = std::string("section-name");
            auto log = SpyLog{};
            auto buf = LogBuffer{section_name, &log};

            buf.write();

            auto const n_written = log.begins_.size();
            if (n_written != 1) {
              t.fatalf("Got {} lines, expected 1", n_written);
            }
            auto const line_written = log.begins_.front();
            if (line_written != section_name) {
              t.errorf("Got line '{}', want '{}'", line_written, section_name);
            }
          });

    t.run("write() does not begin log section if already begun", [](Tester &t) {
      auto const name = std::string("section-name");
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

    t.run("write() ends log section", [](Tester &t) {
      auto log = SpyLog{};
      auto buf = LogBuffer{"section-name", &log};

      buf.write();

      if (log.ends_.size() != 1) {
        t.errorf("Got {} ends, want 1", log.ends_.size());
      }
    });

    t.run("record(line) does not write to log", [](Tester &t) {
      auto log = SpyLog{};
      auto buf = LogBuffer{"", &log};

      buf.record("a line to record");

      if (!log.writes_.empty()) {
        t.errorf("Got {} writes, want none", log.writes_.size());
      }
    });

    t.run("write() writes recorded lines to log", [](Tester &t) {
      auto const lines = std::vector<std::string>{
          "recorded line 1",
          "recorded line 2",
          "recorded line 3",
          "recorded line 4",
      };
      auto log = SpyLog{};
      auto buf = LogBuffer{"", &log};

      for (auto const &line : lines) {
        buf.record(line);
      }

      if (!log.writes_.empty()) {
        t.errorf("Wrote {} lines before write()", log.writes_.size());
      }

      buf.write();

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
