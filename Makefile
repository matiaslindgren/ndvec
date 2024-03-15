CXXFLAGS ?= -std=c++23 -stdlib=libc++ -O3 -Wall -Wpedantic -Werror

MAIN  := ./main.cpp
TEST  := ./test.cpp
NDVEC := ./ndvec.hpp
CODE  := $(MAIN) $(TEST) $(NDVEC)

.PHONY: clean
clean:
	$(RM) main test

.PHONY: fmt
fmt: $(CODE)
	@clang-format-18 --verbose -i $^

$(subst .cpp,,$(MAIN) $(TEST)): % : %.cpp $(NDVEC)
	$(CXX) $(CXXFLAGS) -I $(NDVEC) $< -o $@ -lc++
