#include "dheunit/logger.h"
#include "dheunit/test.h"

namespace dhe {
namespace unit {

struct LogSuite : public Suite {
  LogSuite() : Suite("Log") {}

  void run(Tester &t) override { t.run("write()", write_tests); }

  static inline void write_tests(Tester &t) {}
};

static LogSuite __attribute__((unused)) _{};

} // namespace unit
} // namespace dhe
