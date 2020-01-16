#include <functional>
#include <string>

namespace dhe {
namespace unit {
  /**
   * Creates a context in which to run tests.
   * <p>This function returns a meaningless value so that you can use it in a static context:
   * <pre>
   * auto _ = describe("My context", …) { … }
   * </pre>
   *
   * @param name the name of the context
   * @param builder adds tests, sub-contexts, setup functions, and cleanup functions to this context
   * @return <tt>true</tt>
   */
  auto describe(const std::string &name, std::function<void()> const &builder) -> bool;

  /**
   * Creates a setup function that runs before each test in the current context.
   *
   * @param setupFunction prepares conditions for the test
   */
  void before(std::function<void()> const &setupFunction);

  /**
   * Creates a test to run in the current context.
   *
   * @param name the name of the test
   * @param body executes the test
   */
  void it(const std::string &name, std::function<void()> const &body);

  /**
   * Creates a teardown function that runs after each test in the current context.
   *
   * @param cleanupFunction cleans up after the test
   */
  void after(std::function<void()> const &cleanupFunction);
} // namespace unit
} // namespace dhe
