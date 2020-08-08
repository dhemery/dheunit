#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace dhe {
namespace unit {
  class Reporter {
  public:
    auto failed() const -> bool;

    auto logs() const -> std::vector<std::string>;

    void log(std::string const &msg);

    void error(std::string const &msg);

    void fatal(std::string const &msg);

    void fail();

    void failNow();

  private:
    bool testFailed{false};
    std::vector<std::string> testLogs{};
  };

  class Test {
  public:
    Test(std::string const &name);
    virtual void run(Reporter &) = 0;

  private:
    std::string name;
  };

  class Suite {
  public:
    void add(Test *test);
    void run();

  private:
    std::vector<Test *> tests{};
  };

  auto suite() -> Suite &;

} // namespace unit
} // namespace dhe