# `ndvec`: constexpr Euclidean vector

[godbolt](https://godbolt.org/z/Wjzb7vsYf)

## Description

Consider this code from main.cpp:
```c++
#include "ndvec.hpp"

int main() {
  ndvec v1(1, -2, 3);
  ndvec v2(-3, 2, -1);
  return (v1 - v2).sum() * v1.distance(v2);
}
```
Compiling with Clang 18
```
make CXX=clang-18 main
```
The program exits with 48
```
./main || echo $?
# output: 48
```
Disassembling the output of Clang 18
```sh
objdump --disassemble main | grep --context 5 '<main>'
```
We see the main function compiles to just `return 48`:
```
 70c:	d503201f 	nop

0000000000000710 <frame_dummy>:
 710:	17ffffdc 	b	680 <register_tm_clones>

0000000000000714 <main>:
 714:	52800600 	mov	w0, #0x30                  	// #48
 718:	d65f03c0 	ret

Disassembly of section .fini:
```

## Run in Docker

```
./run_image.sh
```

## Test

```
make CXX=clang-18 test && ./test
```
