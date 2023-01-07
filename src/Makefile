##
# Semi
#
# @Makefile
# @version 0.1

CXXFLAGS = -std=c++17

LIB_EXT = .so

SOURCES = $(wildcard *.cc)

libsemi$(LIB_EXT): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ -shared $(SOURCES)

# end
