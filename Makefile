CXXFLAGS += -Iinclude -std=c++11 -stdlib=libc++
CXXFLAGS += -MMD -MP

SOURCES = $(wildcard \
		src/*.cpp \
		src/*/*.cpp \
		src/*/*/*.cpp \
		)

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

TEST_SOURCES = $(wildcard \
		test/*.cpp \
		test/*/*.cpp \
		test/*/*/*.cpp \
		)

TEST_OBJECTS := $(patsubst %, build/%.o, $(TEST_SOURCES))

TEST_RUNNER = build/runtests

$(TEST_RUNNER): LDFLAGS += -L. -ldheunit

$(TEST_RUNNER): $(TEST_OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

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

COMPILATION_DATABASE_FILE = compile_commands.json

COMPILATION_DATABASE_JSONS := $(patsubst %, build/%.json, $(SOURCES) $(TEST_SOURCES))

$(COMPILATION_DATABASE_FILE): $(COMPILATION_DATABASE_JSONS)
	sed -e '1s/^/[/' -e '$$s/,$$/]/' $^ | json_pp > $@

HEADERS = $(wildcard \
		include/*.h \
		src/*.h \
		src/*/*.h \
		src/*/*/*.h \
		)

.PHONY: format
format:
	clang-format -i -style=file $(HEADERS) $(SOURCES) $(TEST_SOURCES)

.PHONY: tidy
tidy: $(COMPILATION_DATABASE_FILE)
	clang-tidy -header-filter=*/.* -p=build $(HEADERS) $(SOURCES) $(TEST_SOURCES)




########################################################################
#
# General rules
#
########################################################################

DEPENDENCIES = $(OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)
-include $(DEPENDENCIES)

build/%.o: %
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/%.json: %
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MJ $@ -c -o build/$^.o $^

clean:
	rm -rf build
	rm -f libdheunit.a
	rm -rf $(COMPILATION_DATABASE_FILE)
