CXXFLAGS += -Iinclude -std=c++11 -stdlib=libc++
CXXFLAGS += -MMD -MP

SOURCES = $(shell find src -name "*.cpp")

OBJECTS := $(patsubst %, build/%.o, $(SOURCES))

TARGET = libdheunit.a
$(TARGET): $(OBJECTS)
	ar rcu $@ $^
	ranlib $@

.PHONY: all
all: testrunner




########################################################################
#
# Build and run tests
#
########################################################################

TEST_SOURCES = $(shell find test -name "*.cpp")

TEST_OBJECTS := $(patsubst %, build/%.o, $(TEST_SOURCES))

TEST_RUNNER = build/runtests

$(TEST_RUNNER): $(TEST_OBJECTS) $(TARGET)
	$(CXX) -o $@ $(TEST_OBJECTS) -L. -ldheunit $(LDFLAGS)

.PHONY: testrunner
testrunner: $(TARGET) $(TEST_RUNNER)

.PHONY: test
test: testrunner
	$(TEST_RUNNER)




########################################################################
#
# Analyze and format the code
#
########################################################################

HEADERS = $(shell find include src -name "*.h")

.PHONY: format
format:
	clang-format -i -style=file $(HEADERS) $(SOURCES) $(TEST_SOURCES)

COMPILATION_DB = compile_commands.json

COMPILATION_DB_ENTRIES := $(patsubst %, build/%.json, $(SOURCES) $(TEST_SOURCES))

$(COMPILATION_DB): $(COMPILATION_DB_ENTRIES)
	sed -e '1s/^/[/' -e '$$s/,$$/]/' $^ | json_pp > $@

.PHONY: tidy
tidy: $(COMPILATION_DB)
	clang-tidy -header-filter='^(src|include|test)/' -p=build $(SOURCES) $(TEST_SOURCES)




########################################################################
#
# General rules
#
########################################################################

-include $(OBJECTS:.o=.d)
-include $(TEST_OBJECTS:.o=.d)

build/%.o: %
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/%.json: %
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MJ $@ -c -o build/$^.o $^

clean:
	rm -rf build
	rm -f libdheunit.a
	rm -rf $(COMPILATION_DB)
