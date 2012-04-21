#
# created by Bartek 'BaSz' Szurgot (http://www.baszerr.eu)
#

CXX:=g++-4.7
CXXFLAGS:=-std=c++11

#CXX:=g++-4.6
#CXXFLAGS:=-std=c++0x


# common flags
CXXFLAGS+=-Wall -O3 -DNDEBUG -pipe -MD

SRCS:=$(wildcard *.cpp)
OBJS:=$(SRCS:.cpp=.o)
DEPS:=$(SRCS:.cpp=.d)

.PHONY: all
all: example.out

-include $(DEPS)

%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -c $*.cpp

example.out: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -vf *.o *.out *.d
