#include "dheunit/log.h"
#include "dheunit/test.h"

#include <sstream>

namespace dhe {
namespace unit {
namespace log {
namespace test {

struct DefaultLogSuite : public Suite {
  DefaultLogSuite() : Suite("DefaultLog") {}

  void run(Tester &t) override {
    t.run("write() writes str as line", [](Tester &t) {
      auto out = std::ostringstream{};
      auto log = DefaultLog{out, true};

      auto const line = std::string{"the logged line of text"};
      log.write(Level::Info, line);

      auto const got = out.str();
      auto const want = line + '\n';
      if (got != want) {
        t.errorf("Got '{}', want '{}'", got, want);
      }
    });

    t.run("start() writes str as line", [](Tester &t) {
      auto out = std::ostringstream{};
      auto log = DefaultLog{out, true};

      auto const name = std::string{"the name"};
      log.start(name);

      auto const got = out.str();
      auto const want = name + '\n';
      if (got != want) {
        t.errorf("Got '{}', want '{}'", got, want);
      }
    });

    t.run("each start() increases indent by 4 spaces", [](Tester &t) {
      auto out = std::ostringstream{};
      auto log = DefaultLog{out, true};

      log.start("begin1");
      log.write(Level::Info, "line1");
      log.start("begin2");
      log.write(Level::Info, "line2");
      log.start("begin3");
      log.write(Level::Info, "line3");
      log.start("begin4");
      log.write(Level::Info, "line4");
      log.start("begin5");
      log.write(Level::Info, "line5");

      auto expected_output = std::ostringstream{};
      expected_output << "begin1" << '\n';
      expected_output << "    line1" << '\n';
      expected_output << "    begin2" << '\n';
      expected_output << "        line2" << '\n';
      expected_output << "        begin3" << '\n';
      expected_output << "            line3" << '\n';
      expected_output << "            begin4" << '\n';
      expected_output << "                line4" << '\n';
      expected_output << "                begin5" << '\n';
      expected_output << "                    line5" << '\n';

      if (out.str() != expected_output.str()) {
        auto error = std::ostringstream{};
        error << "Got" << '\n';
        error << "=====" << '\n';
        error << out.str() << '\n';
        error << "=====" << '\n';
        error << "Want" << '\n';
        error << "=====" << '\n';
        error << expected_output.str() << '\n';
        error << "=====" << '\n';
        t.error(error.str());
      }
    });

    t.run("each end() decreases indent by 4 spaces", [](Tester &t) {
      auto out = std::ostringstream{};
      auto log = DefaultLog{out, true};

      log.start("begin1");
      log.start("begin2");
      log.start("begin3");
      log.start("begin4");
      out.str("");

      log.write(Level::Info, "line1");
      log.end();
      log.write(Level::Info, "line2");
      log.end();
      log.write(Level::Info, "line3");
      log.end();
      log.write(Level::Info, "line4");
      log.end();
      log.write(Level::Info, "line5");

      auto expected_output = std::ostringstream{};
      expected_output << "                line1" << '\n';
      expected_output << "            line2" << '\n';
      expected_output << "        line3" << '\n';
      expected_output << "    line4" << '\n';
      expected_output << "line5" << '\n';

      if (out.str() != expected_output.str()) {
        auto error = std::ostringstream{};
        error << "Got" << '\n';
        error << "=====" << '\n';
        error << out.str() << '\n';
        error << "=====" << '\n';
        error << "Want" << '\n';
        error << "=====" << '\n';
        error << expected_output.str() << '\n';
        error << "=====" << '\n';
        t.error(error.str());
      }
    });
  }
};

static auto _ = DefaultLogSuite{};
} // namespace test
} // namespace log
} // namespace unit
} // namespace dhe
