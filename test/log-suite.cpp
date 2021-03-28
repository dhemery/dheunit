#include "dheunit/assertions.h"
#include "dheunit/logger.h"
#include "dheunit/test.h"

namespace dhe {
namespace unit {
namespace test {
struct StreamLogSuite : public Suite {
  StreamLogSuite() : Suite("StreamLog.write()") {}

  void run(Tester &t) override {
    t.run("writes the line to the stream", [](Tester &t) {
      auto const line = std::string{"the logged line of text"};
      auto output = std::ostringstream{};
      auto log = log::StreamLog{output};

      log.write(line);

      t.assert_that(output.str(), is_equal_to(line + '\n'));
    });
  }
};

static StreamLogSuite __attribute__((unused)) _{};
} // namespace test
} // namespace unit
} // namespace dhe
