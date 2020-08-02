
FLAGS += -Isrc
LDFLAGS +=
SOURCES = $(wildcard \
		src/*.cpp \
		src/*/*.cpp \
		src/*/*/*.cpp \
		)

OBJECTS := $(patsubst %.cpp,build/%.o,$(SOURCES))

lib: libdheunit.a

build:
	@mkdir -p $@

build/src/%.o: src/%.cpp
	$(CXX) $(FLAGS) -o $@ -c $^


$(OBJECTS): build

libdheunit.a: $(OBJECTS)
	$(CXX) $(FLAGS) -o $@ $^ $(LDFLAGS)




########################################################################
#
# Analyze and format the code
#
########################################################################

COMPILATION_DATABASE_FILE = compile_commands.json

COMPILATION_DATABASE_JSONS := $(patsubst %, build/%.json, $(SOURCES) $(DOCTEST_SOURCES))

build/src/%.json: src/%
	@mkdir -p $(@D)
	$(CXX) $(FLAGS) -MJ $@ -c -o build/$^.o $^

build/test/%.json: test/%
	@mkdir -p $(@D)
	$(CXX) $(FLAGS) -MJ $@ -c -o build/$^.o $^

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
