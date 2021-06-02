#include "dheunit/format.h"
#include "dheunit/test.h"

#include <functional>
#include <strstream>
#include <vector>

namespace dhe {
namespace unit {
namespace format {
namespace test {

struct FormatTest {
  std::string name_;
  std::function<std::string()> action_;
  std::string want_;
};

static auto join_tests = std::vector<FormatTest>{
    {
        .name_ = "0 args",
        .action_ = []() -> std::string { return joined(); },
        .want_ = "",
    },
    {
        .name_ = "1 arg",
        .action_ = []() -> std::string { return joined(1); },
        .want_ = "1",
    },
    {
        .name_ = "2 args",
        .action_ = []() -> std::string { return joined(1, 2); },
        .want_ = "1 2",
    },
    {
        .name_ = "n args",
        .action_ = []() -> std::string { return joined(1, 2, 3, 4, 5); },
        .want_ = "1 2 3 4 5",
    },
    {
        .name_ = "various types",
        .action_ = []() -> std::string {
          return joined(true, 1, 2.3F, "three");
        },
        .want_ = "true 1 2.3 three",
    },
};

static auto format_tests = std::vector<FormatTest>{
    {
        .name_ = "empty format",
        .action_ = []() -> std::string { return formatted(""); },
        .want_ = "",
    },
    {
        .name_ = "0 anchors",
        .action_ = []() -> std::string { return formatted("0 anchors"); },
        .want_ = "0 anchors",
    },
    {
        .name_ = "1 anchor",
        .action_ = []() -> std::string { return formatted("{} anchor", 1); },
        .want_ = "1 anchor",
    },
    {
        .name_ = "2 anchors",
        .action_ = []() -> std::string {
          return formatted("one:{} two:{}", 1, 2);
        },
        .want_ = "one:1 two:2",
    },
    {
        .name_ = "n anchors",
        .action_ = []() -> std::string {
          return formatted("values {}+{}+{}+{}+{}", 1, 2, 3, 4, 5);
        },
        .want_ = "values 1+2+3+4+5",

    },
    {
        .name_ = "various types",
        .action_ = []() -> std::string {
          return formatted("bool:{} int:{} float:{} string:{}", true, 1, 2.3F,
                           "three");
        },
        .want_ = "bool:true int:1 float:2.3 string:three",
    },
};

struct FormatSuite : public Suite {
  FormatSuite() : Suite("format") {}

  void run(Tester &t) override {
    t.run("joined()", [](Tester &t) {
      for (auto &test : join_tests) {
        t.run(test.name_, [test](Tester &t) {
          auto const got = test.action_();
          if (got != test.want_) {
            t.errorf(R"(Got "{}", want "{}")", got, test.want_);
          }
        });
      }
    });

    t.run("formatted()", [](Tester &t) {
      for (auto &test : format_tests) {
        t.run(test.name_, [test](Tester &t) {
          auto const got = test.action_();
          if (got != test.want_) {
            t.errorf(R"(Got "{}", want "{}")", got, test.want_);
          }
        });
      }
    });
  }
  //
  //  static void write_tests(Tester &t) {
  //    t.run("one arg", [](Tester &t) {
  //      auto out = std::ostringstream{};
  //
  //      format::write(out, true);
  //
  //      auto const got = out.str();
  //
  //      auto constexpr want = "true";
  //      if (got != want) {
  //        t.errorf(R"(Got "{}", want "{}")", got, want);
  //      }
  //    });
  //
  //    t.run("n args", [](Tester &t) {
  //      auto out = std::ostringstream{};
  //
  //      format::write(out, true, 2, "fred");
  //
  //      auto const got = out.str();
  //
  //      auto constexpr want = "true 2 fred";
  //      if (got != want) {
  //        t.errorf(R"(Got "{}", want "{}")", got, want);
  //      }
  //    });
  //  }
  //
  //  static void writef_tests(Tester &t) {
  //    t.run("null format with no args", [](Tester &t) {
  //      auto out = std::ostringstream{};
  //      out << std::boolalpha;
  //
  //      try {
  //        format::writef(out, nullptr);
  //        t.error("Succeeded, but want FormatError");
  //      } catch (format::FormatError & *//*expected*//*) {
  //      }
  //    });
  //
  //    t.run("null format with n args", [](Tester &t) {
  //      auto out = std::ostringstream{};
  //      out << std::boolalpha;
  //
  //      try {
  //        format::writef(out, nullptr, "arg1", "arg2", "arg3", "arg4");
  //        t.error("Succeeded, but want FormatError");
  //      } catch (format::FormatError & *//*expected*//*) {
  //      }
  //    });
  //
  //    t.run("empty format", [](Tester &t) {
  //      auto out = std::ostringstream{};
  //      out << std::boolalpha;
  //
  //      format::writef(out, "");
  //      auto const str = out.str();
  //
  //      if (!str.empty()) {
  //        t.errorf(R"(Got "{}", want "")", str);
  //      }
  //    });
  //
  //    t.run("no anchors", [](Tester &t) {
  //      auto out = std::ostringstream{};
  //      out << std::boolalpha;
  //
  //      auto constexpr format = "a format with no anchors";
  //      format::writef(out, format);
  //      auto const str = out.str();
  //
  //      if (str != format) {
  //        t.errorf(R"(Got "{}", want "{}")", str, format);
  //      }
  //    });
  //
  //    t.run("1 anchor 1 arg", [](Tester &t) {
  //      auto out = std::ostringstream{};
  //      out << std::boolalpha;
  //
  //      format::writef(out, "prefix {} suffix", "arg1");
  //      auto const str = out.str();
  //
  //      auto constexpr want = "prefix arg1 suffix";
  //      if (str != want) {
  //        t.errorf(R"(Got "{}", want "{}")", str, want);
  //      }
  //    });
  //
  //    t.run("n anchors n args", [](Tester &t) {
  //      auto out = std::ostringstream{};
  //      out << std::boolalpha;
  //
  //      format::writef(out, "{},{},{}", "arg1", "arg2", "arg3");
  //      auto const str = out.str();
  //
  //      auto constexpr want = "arg1,arg2,arg3";
  //      if (str != want) {
  //        t.errorf(R"(Got "{}", want "{}")", str, want);
  //      }
  //    });
  //
  //    t.run("more anchors than args", [](Tester &t) {
  //      auto out = std::ostringstream{};
  //      out << std::boolalpha;
  //
  //      try {
  //        format::writef(out, "{},{},{}", "arg1", "arg2");
  //        t.error("Succeeded, but want FormatError");
  //      } catch (format::FormatError & *//*expected*//*) {
  //      }
  //    });
  //
  //    t.run("more args than anchors", [](Tester &t) {
  //      auto out = std::ostringstream{};
  //      out << std::boolalpha;
  //
  //      try {
  //        format::writef(out, "{},{},{}", "arg1", "arg2", "arg3", "arg4");
  //        t.error("Succeeded, but want FormatError");
  //      } catch (format::FormatError & *//*expected*//*) {
  //      }
  //    });
  //
  //    t.run("opening brace precedes anchor", [](Tester &t) {
  //      auto out = std::ostringstream{};
  //      out << std::boolalpha;
  //
  //      format::writef(out, "{{}foo", "arg1");
  //      auto const str = out.str();
  //
  //      auto constexpr want = "{arg1foo";
  //      if (str != want) {
  //        t.errorf(R"(Got "{}", want "{}")", str, want);
  //      }
  //    });
  //  }
};

static auto _ = FormatSuite{};

} // namespace test
} // namespace format
} // namespace unit
} // namespace dhe
