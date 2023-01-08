##
# Semi
#
# @Makefile
# @version 0.1

.PHONY: clean all FORCE
.DEFAULT_GOAL = all

CXXFLAGS = -fPIC -shared -std=c++17

LIB_EXT = .so

SOURCES = $(wildcard *.cc)

clean: FORCE
	$(RM) -f libsemi$(LIB_EXT)

libsemi$(LIB_EXT): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

all: libsemi$(LIB_EXT)

# end
