CXX = g++
CXXFLAGS = -std=c++11 -O3 -DNDEBUG -Wall -Wextra -pedantic

.PHONY: all
all: Dinic EK MPM

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
