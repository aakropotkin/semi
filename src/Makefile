# ============================================================================ #
#
# Semi
#
# @Makefile
# @version 0.1
#
# ---------------------------------------------------------------------------- #

.PHONY: clean all FORCE
.DEFAULT_GOAL = all

FLEX  = flex
BISON = bison


# ---------------------------------------------------------------------------- #

EXTRA_CXXFLAGS = -Wall -Wpedantic -Wextra
CXXFLAGS       = $(EXTRA_CXXFLAGS) -std=c++2a
LIB_CXXFLAGS   = -fPIC -shared $(CXXFLAGS)
BIN_CXXFLAGS   = $(CXXFLAGS)

LIB_EXT = .so

SOURCES = semver.cc comparator.cc range.cc


# ---------------------------------------------------------------------------- #

semver.cc: semver.hh regexes.hh comparator.hh range.hh

libsemi$(LIB_EXT): $(SOURCES)
	$(CXX) $(LIB_CXXFLAGS) -o $@ $(SOURCES)

test: test.cc libsemi$(LIB_EXT)
	$(CXX) $(BIN_CXXFLAGS) -o $@ $^ -L$$PWD -lsemi

all: libsemi$(LIB_EXT) test


# ---------------------------------------------------------------------------- #

lexer.hpp lexer.cpp: lexer.l
	$(FLEX) --header-file=lexer.hpp -o lexer.cpp $<

parser.hpp parser.cpp: grammar.y
	$(BISON) -d -v -o parser.cpp $<

simple: lexer.cpp parser.cpp
	$(CXX) $(BIN_CXXFLAGS) -o $@ $^


# ---------------------------------------------------------------------------- #

clean: FORCE
	$(RM) -f libsemi$(LIB_EXT)
	$(RM) -f test
	$(RM) -f lexer.cpp lexer.hpp parser.hpp parser.cpp parser.output simple


# ---------------------------------------------------------------------------- #
#
#
#
# ============================================================================ #
