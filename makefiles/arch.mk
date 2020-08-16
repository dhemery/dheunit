MACHINE = $(shell $(CC) -dumpmachine)
ifneq (, $(findstring apple, $(MACHINE)))
        ARCH_MAC := 1
        ARCH := mac
else ifneq (, $(findstring mingw, $(MACHINE)))
        ARCH_WIN := 1
        ARCH := win
        ifneq ( ,$(findstring x86_64, $(MACHINE)))
                ARCH_WIN_64 := 1
                BITS := 64
        else ifneq (, $(findstring i686, $(MACHINE)))
                ARCH_WIN_32 := 1
                BITS := 32
        endif
else ifneq (, $(findstring linux, $(MACHINE)))
        ARCH_LIN := 1
        ARCH := lin
else
$(error Could not determine architecture of $(MACHINE).)
endif

# Architecture-dependent flags
ifdef ARCH_LIN
        CXXFLAGS += -Wsuggest-override
endif
ifdef ARCH_MAC
        CXXFLAGS += -stdlib=libc++
        LDFLAGS += -stdlib=libc++
        MAC_SDK_FLAGS = -mmacosx-version-min=10.7
        FLAGS += $(MAC_SDK_FLAGS)
        LDFLAGS += $(MAC_SDK_FLAGS)
endif
ifdef ARCH_WIN
        FLAGS += -D_USE_MATH_DEFINES
        CXXFLAGS += -Wsuggest-override
endif

