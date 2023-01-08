##
# Semi
#
# @Makefile
# @version 0.1

.PHONY: clean all FORCE
.DEFAULT_GOAL = all

CXXFLAGS = -std=c++17
LIB_CXXFLAGS = -fPIC -shared $(CXXFLAGS)
BIN_CXXFLAGS = $(CXXFLAGS)

LIB_EXT = .so

SOURCES = semver.cc

semver.cc: semver.hh regexes.hh

libsemi$(LIB_EXT): $(SOURCES)
	$(CXX) $(LIB_CXXFLAGS) -o $@ $(SOURCES)

test: test.cc libsemi$(LIB_EXT)
	$(CXX) $(BIN_CXXFLAGS) -o $@ $^ -L$$PWD -lsemi

all: libsemi$(LIB_EXT) test

clean: FORCE
	$(RM) -f libsemi$(LIB_EXT)

# end
