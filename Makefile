CXX ?= g++
CPPFLAGS += -Iinclude
CXXFLAGS ?= -std=c++11 -Wall -Wextra -Wpedantic -O2 -g

SOURCES := $(wildcard examples/*.cpp)
TARGETS := $(patsubst examples/%.cpp,build/%,$(SOURCES))
HEADERS := $(wildcard include/*.hpp)

.PHONY: all clean
all: $(TARGETS)

$(TARGETS): build/%: examples/%.cpp $(HEADERS) Makefile | build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@

build:
	mkdir -p $@

clean:
	$(RM) $(TARGETS)
