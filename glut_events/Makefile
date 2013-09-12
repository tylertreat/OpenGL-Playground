#! /usr/bin/make

.NULL: .exe

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
TARGETS = $(basename $(SOURCES))

CXXOPTS = -mwin32 -g 
CXXINCS = -I../include
CXXFLAGS = $(CXXOPTS) $(CXXDEFS) $(CXXINCS)

LDDIRS = -L../lib/windows
LDLIBS = -lfreeglut -lopengl32 -lgdi32 -lwinmm
LDFLAGS = $(LDOPTS) $(LDDIRS) $(LDLIBS)

DIRT = $(wildcard *.o *.i *~ */*~ *.log)

#-----------------------------------------------------------------------------

.PHONY: Makefile

default all: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

#-----------------------------------------------------------------------------

%.i: %.cpp
	$(CXX) -E $(CXXFLAGS) $< | uniq > $@

#-----------------------------------------------------------------------------

clean:
	$(RM) $(DIRT)

rmtargets:
	$(RM) $(TARGETS)

clobber: clean rmtargets
