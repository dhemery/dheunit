#include "dheunit/assertions.h"
#include "dheunit/internal/stream-log.h"
#include "dheunit/test.h"

#include <sstream>

namespace dhe {
namespace unit {
namespace log {
namespace test {

struct StreamLogSuite : public Suite {
  StreamLogSuite() : Suite("StreamLog") {}

  void run(Tester &t) override {
    t.run("write() writes str as line", [](Tester &t) {
      auto out = std::ostringstream{};
      auto log = StreamLog{out};

      auto const line = std::string{"the logged line of text"};
      log.write(line);

      t.assert_that(out.str(), is_equal_to(line + '\n'));
    });

    t.run("begin() writes str as line", [](Tester &t) {
      auto out = std::ostringstream{};
      auto log = StreamLog{out};

      auto const name = std::string{"the name"};
      log.begin(name);

      t.assert_that(out.str(), is_equal_to(name + '\n'));
    });

    t.run("each begin() increases indent by 4 spaces", [](Tester &t) {
      auto out = std::ostringstream{};
      auto log = StreamLog{out};

      log.begin("begin1");
      log.write("line1");
      log.begin("begin2");
      log.write("line2");
      log.begin("begin3");
      log.write("line3");
      log.begin("begin4");
      log.write("line4");
      log.begin("begin5");
      log.write("line5");

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
      auto log = StreamLog{out};

      log.begin("begin1");
      log.begin("begin2");
      log.begin("begin3");
      log.begin("begin4");
      out.str("");

      log.write("line1");
      log.end();
      log.write("line2");
      log.end();
      log.write("line3");
      log.end();
      log.write("line4");
      log.end();
      log.write("line5");

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

static StreamLogSuite __attribute__((unused)) _{};
} // namespace test
} // namespace log
} // namespace unit
} // namespace dhe
