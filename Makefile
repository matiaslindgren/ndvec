CXXFLAGS ?= -std=c++23 -stdlib=libc++ -O3 -Wall -Wpedantic -Werror

MAIN  := ./main.cpp
NDVEC := ./ndvec.hpp
CODE  := $(MAIN) $(NDVEC)
OUT   := ./example

.PHONY: clean
clean:
	$(RM) $(OUT)

.PHONY: fmt
fmt: $(CODE)
	@clang-format-18 --verbose -i $^

$(OUT): $(CODE)
	$(CXX) $(CXXFLAGS) -I $(NDVEC) $(MAIN) -o $@
