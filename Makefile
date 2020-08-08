FLAGS += -Isrc
LDFLAGS +=
CXXFLAGS += -std=c++11 -stdlib=libc++
CXXFLAGS += $(FLAGS)

SOURCES = $(wildcard \
		src/*.cpp \
		src/*/*.cpp \
		src/*/*/*.cpp \
		)

OBJECTS := $(patsubst %, build/%.o, $(SOURCES))

lib: libdheunit.a

build/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

libdheunit.a: $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)




########################################################################
#
# Analyze and format the code
#
########################################################################

COMPILATION_DATABASE_FILE = compile_commands.json

COMPILATION_DATABASE_JSONS := $(patsubst %, build/%.json, $(SOURCES))

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
