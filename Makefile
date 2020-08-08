CXXFLAGS += -Isrc -std=c++11 -stdlib=libc++

SOURCES = $(wildcard \
		src/*.cpp \
		src/*/*.cpp \
		src/*/*/*.cpp \
		)

OBJECTS := $(patsubst %, build/%.o, $(SOURCES))

all: lib

lib: libdheunit.a

build/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

libdheunit.a: $(OBJECTS)
	ar rcu $@ $^
	ranlib $@

clean:
	rm -rf build
	rm libdheunit.a




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

build/test/%.cpp.o: test/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/runtests: $(TEST_OBJECTS) libdheunit.a
	$(CXX)  -L. $(LDFLAGS) -o $@ $< -ldheunit

test: build/runtests
	build/runtests


########################################################################
#
# Analyze and format the code
#
########################################################################

COMPILATION_DATABASE_FILE = compile_commands.json

COMPILATION_DATABASE_JSONS := $(patsubst %, build/%.json, $(SOURCES))

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
	clang-format -i -style=file $(HEADERS) $(SOURCES)

tidy: $(COMPILATION_DATABASE_FILE)
	clang-tidy -header-filter=src/.* -p=build $(SOURCES)

cleancdb:
	rm -rf $(COMPILATION_DATABASE_FILE) $(COMPILATION_DATABASE_JSONS)

clean: cleancdb

.PHONY: format tidy cleancdb
