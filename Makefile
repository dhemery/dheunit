CXXFLAGS += -Iinclude -std=c++11 -stdlib=libc++
CXXFLAGS += -MMD -MP

SOURCES = $(wildcard \
		src/*.cpp \
		src/*/*.cpp \
		src/*/*/*.cpp \
		)

OBJECTS := $(patsubst %, build/%.o, $(SOURCES))

lib: libdheunit.a

all: lib build/runtests

build/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

libdheunit.a: $(OBJECTS)
	ar rcu $@ $^
	ranlib $@

clean:
	rm -rf build
	rm -f libdheunit.a




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

build/runtests: $(TEST_OBJECTS)
	$(CXX)  -L. $(LDFLAGS) -o $@ $^ -ldheunit

test: build/runtests
	build/runtests


########################################################################
#
# Analyze and format the code
#
########################################################################

COMPILATION_DATABASE_FILE = compile_commands.json

COMPILATION_DATABASE_JSONS := $(patsubst %, build/%.json, $(SOURCES) $(TEST_SOURCES))

build/src/%.json: src/%
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MJ $@ -c -o build/$^.o $^

build/test/%.json: test/%
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MJ $@ -c -o build/$^.o $^

$(COMPILATION_DATABASE_FILE): $(COMPILATION_DATABASE_JSONS)
	sed -e '1s/^/[/' -e '$$s/,$$/]/' $^ | json_pp > $@

HEADERS = $(wildcard \
		src/*.h \
		src/*/*.h \
		src/*/*/*.h \
		)

format:
	clang-format -i -style=file $(HEADERS) $(SOURCES) $(TEST_SOURCES)

tidy: $(COMPILATION_DATABASE_FILE)
	clang-tidy -header-filter=src/.* -p=build $(SOURCES) $(TEST_SOURCES)

cleancdb:
	rm -rf $(COMPILATION_DATABASE_FILE) $(COMPILATION_DATABASE_JSONS)

clean: cleancdb

.PHONY: format tidy cleancdb
-include $(OBJECTS:.o=.d)
-include $(TEST_OBJECTS:.o=.d)
