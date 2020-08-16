#include "dheunit.h"

namespace dhe {
namespace unit {
  struct LogEntryLogSuite : public Suite {
    LogEntryLogSuite() : Suite{"dhe::unit::LogEntry write"} {}
    void addTests(TestMap &test) override {
      test["one arg"] = [](Tester &t) {
        auto entry = LogEntry{};
        entry.write(true);
        auto const str = entry.str();
        if (str != "true") {
          t.errorf(R"(Got "{}", want "true")", str);
        }
      };

      test["n args"] = [](Tester &t) {
        auto entry = LogEntry{};
        entry.write(true, 2, "fred");

        auto const str = entry.str();

        auto constexpr want = "true 2 fred";
        if (str != want) {
          t.errorf(R"(Got "{}", want "{}")", str, want);
        }
      };
    }
  };
  static LogEntryLogSuite logSuite __attribute__((unused)){};

  struct LogEntryLogfSuite : public Suite {
    LogEntryLogfSuite() : Suite{"dhe::unit::LogEntry writef"} {}
    void addTests(TestMap &test) override {
      test["null format"] = [](Tester &t) {
        auto entry = LogEntry{};

        entry.writef(nullptr);
        auto const str = entry.str();

        if (!str.empty()) {
          t.errorf(R"(Got "{}", want "")", str);
        }
      };

      test["empty format"] = [](Tester &t) {
        auto entry = LogEntry{};

        entry.writef("");
        auto const str = entry.str();

        if (!str.empty()) {
          t.errorf(R"(Got "{}", want "")", str);
        }
      };

      test["no anchors"] = [](Tester &t) {
        auto entry = LogEntry{};

        auto constexpr format = "a format with no anchors";
        entry.writef(format);
        auto const str = entry.str();

        if (str != format) {
          t.errorf(R"(Got "{}", want "{}")", str, format);
        }
      };

      test["1 anchor 1 arg"] = [](Tester &t) {
        auto entry = LogEntry{};

        entry.writef("prefix {} suffix", "arg1");
        auto const str = entry.str();

        auto constexpr want = "prefix arg1 suffix";
        if (str != want) {
          t.errorf(R"(Got "{}", want "{}")", str, want);
        }
      };

      test["n anchors n args"] = [](Tester &t) {
        auto entry = LogEntry{};

        entry.writef("{},{},{}", "arg1", "arg2", "arg3");
        auto const str = entry.str();

        auto constexpr want = "arg1,arg2,arg3";
        if (str != want) {
          t.errorf(R"(Got "{}", want "{}")", str, want);
        }
      };

      test["more anchors than args"] = [](Tester &t) {
        auto entry = LogEntry{};

        try {
          entry.writef("{},{},{}", "arg1", "arg2");
          t.error("Succeeded, but want FormatError");
        } catch (LogEntry::FormatError & /*expected*/) {
        }
      };

      test["more args than anchors"] = [](Tester &t) {
        auto entry = LogEntry{};

        try {
          entry.writef("{},{},{}", "arg1", "arg2", "arg3", "arg4");
          t.error("Succeeded, but want FormatError");
        } catch (LogEntry::FormatError & /*expected*/) {
        }
      };

      test["opening brace precedes anchor"] = [](Tester &t) {
        auto entry = LogEntry{};

        entry.writef("{{}foo", "arg1");
        auto const str = entry.str();

        auto constexpr want = "{arg1foo";
        if (str != want) {
          t.errorf(R"(Got "{}", want "{}")", str, want);
        }
      };

      // TODO: null format then args
    }
  };
  static LogEntryLogfSuite logfSuite __attribute__((unused)){};

} // namespace unit
} // namespace dhe
