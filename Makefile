CXX=g++
IDIR =include/
CXXFLAGS=-std=c++20 -O3 -I$(IDIR)
MAKEFLAGS += --always-make

%:
	$(CXX) $@/sol.cpp -o $@/sol $(CXXFLAGS)

clean:
	rm -f */*.out
