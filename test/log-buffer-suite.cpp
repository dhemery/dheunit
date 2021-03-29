#include "dheunit/internal/log-buffer.h"

#include "dheunit/assertions.h"
#include "dheunit/test.h"

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
    t.run("announce() calls log.begin(name) if not already announced",
          [](Tester &t) {
            auto const name = std::string("log-buffer-name");
            auto log = SpyLog{};
            auto buf = LogBuffer{name, &log};

            buf.announce();

            t.assert_that_f(log.begins_.size(), is_equal_to(1));
            t.assert_that(log.begins_.front(), is_equal_to(name));
          });

    t.run("announce() does not call log.begin(name) if already announced",
          [](Tester &t) {
            auto const name = std::string("log-buffer-name");
            auto log = SpyLog{};
            auto buf = LogBuffer{name, &log};

            buf.announce();
            log.begins_.clear();

            buf.announce();
            buf.announce();
            buf.announce();
            buf.announce();

            t.assert_that(log.begins_.size(), is_equal_to(0));
          });
  }
};

LogBufferSuite __attribute__((unused)) _{};
} // namespace test
} // namespace log
} // namespace unit
} // namespace dhe
