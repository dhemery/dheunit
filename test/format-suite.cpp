#include "dheunit/format.h"
#include "dheunit/test.h"

#include <ios>

namespace dhe {
namespace unit {
namespace test {

struct FormatSuite : public Suite {
  FormatSuite() : Suite("format::") {}

  void run(Tester &t) override {
    t.run("write()", write_tests);
    t.run("writef()", writef_tests);
  }

  static void write_tests(Tester &t) {
    t.run("one arg", [](Tester &t) {
      auto out = std::ostringstream{};
      out << std::boolalpha;

      format::write(out, true);

      auto const got = out.str();

      auto constexpr want = "true";
      if (got != want) {
        t.errorf(R"(Got "{}", want "{}")", got, want);
      }
    });

    t.run("n args", [](Tester &t) {
      auto out = std::ostringstream{};
      out << std::boolalpha;

      format::write(out, true, 2, "fred");

      auto const got = out.str();

      auto constexpr want = "true 2 fred";
      if (got != want) {
        t.errorf(R"(Got "{}", want "{}")", got, want);
      }
    });
  }

  static void writef_tests(Tester &t) {
    t.run("null format with no args", [](Tester &t) {
      auto out = std::ostringstream{};
      out << std::boolalpha;

      try {
        format::writef(out, nullptr);
        t.error("Succeeded, but want FormatError");
      } catch (format::FormatError & /*expected*/) {
      }
    });

    t.run("null format with n args", [](Tester &t) {
      auto out = std::ostringstream{};
      out << std::boolalpha;

      try {
        format::writef(out, nullptr, "arg1", "arg2", "arg3", "arg4");
        t.error("Succeeded, but want FormatError");
      } catch (format::FormatError & /*expected*/) {
      }
    });

    t.run("empty format", [](Tester &t) {
      auto out = std::ostringstream{};
      out << std::boolalpha;

      format::writef(out, "");
      auto const str = out.str();

      if (!str.empty()) {
        t.errorf(R"(Got "{}", want "")", str);
      }
    });

    t.run("no anchors", [](Tester &t) {
      auto out = std::ostringstream{};
      out << std::boolalpha;

      auto constexpr format = "a format with no anchors";
      format::writef(out, format);
      auto const str = out.str();

      if (str != format) {
        t.errorf(R"(Got "{}", want "{}")", str, format);
      }
    });

    t.run("1 anchor 1 arg", [](Tester &t) {
      auto out = std::ostringstream{};
      out << std::boolalpha;

      format::writef(out, "prefix {} suffix", "arg1");
      auto const str = out.str();

      auto constexpr want = "prefix arg1 suffix";
      if (str != want) {
        t.errorf(R"(Got "{}", want "{}")", str, want);
      }
    });

    t.run("n anchors n args", [](Tester &t) {
      auto out = std::ostringstream{};
      out << std::boolalpha;

      format::writef(out, "{},{},{}", "arg1", "arg2", "arg3");
      auto const str = out.str();

      auto constexpr want = "arg1,arg2,arg3";
      if (str != want) {
        t.errorf(R"(Got "{}", want "{}")", str, want);
      }
    });

    t.run("more anchors than args", [](Tester &t) {
      auto out = std::ostringstream{};
      out << std::boolalpha;

      try {
        format::writef(out, "{},{},{}", "arg1", "arg2");
        t.error("Succeeded, but want FormatError");
      } catch (format::FormatError & /*expected*/) {
      }
    });

    t.run("more args than anchors", [](Tester &t) {
      auto out = std::ostringstream{};
      out << std::boolalpha;

      try {
        format::writef(out, "{},{},{}", "arg1", "arg2", "arg3", "arg4");
        t.error("Succeeded, but want FormatError");
      } catch (format::FormatError & /*expected*/) {
      }
    });

    t.run("opening brace precedes anchor", [](Tester &t) {
      auto out = std::ostringstream{};
      out << std::boolalpha;

      format::writef(out, "{{}foo", "arg1");
      auto const str = out.str();

      auto constexpr want = "{arg1foo";
      if (str != want) {
        t.errorf(R"(Got "{}", want "{}")", str, want);
      }
    });
  }
};

static auto _ = FormatSuite{};

} // namespace test
} // namespace unit
} // namespace dhe
