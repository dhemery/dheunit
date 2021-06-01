#include "dheunit/internal/buffered-log.h"

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

  void end() override { ends_.push_back(true); }

  std::vector<std::string> begins_{}; // NOLINT
  std::vector<std::string> writes_{}; // NOLINT
  std::vector<bool> ends_{};          // NOLINT
};

class LogBufferSuite : Suite {
public:
  LogBufferSuite() : Suite("LogBuffer") {}

  void run(Tester &t) override {
    t.run("write(line) begins named log section if not already begun",
          [](Tester &t) {
            auto const section_name = std::string("section-name");
            auto log = SpyLog{};
            auto buf = LogBuffer{section_name, &log};

            buf.write("a line to record");

            auto const n_written = log.begins_.size();
            if (n_written != 1) {
              t.fatalf("Got {} lines, expected 1", n_written);
            }
            auto const line_written = log.begins_.front();
            if (line_written != section_name) {
              t.errorf("Got line '{}', want '{}'", line_written, section_name);
            }
          });

    t.run("write(line) does not begin log section if already begun",
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
  }
};

LogBufferSuite __attribute__((unused)) _{};
} // namespace test
} // namespace log
} // namespace unit
} // namespace dhe
