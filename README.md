# `ndvec`: constexpr Euclidean vector

[godbolt](https://godbolt.org/z/zhxMjdPh8)

## Description

Consider this code from main.cpp:
```c++
#include "ndvec.hpp"

int main() {
  using ndvec::ndvec;
  ndvec v1(1, -2, 3);
  ndvec v2(-3, 2, -1);
  ndvec v3(5, 6, 3);
  ndvec v4(1, 2, -3, -2, 4, 10);
  return v4.signum().sum() + (v1 - v2).sum() * v1.distance(v3);
}
```
Compiling with Clang 18
```
make CXX=clang-18 main
```
The program exits with 50
```
./main || echo $?
# output: 50
```
Disassembling the output
```sh
objdump --disassemble main | grep --context 5 '<main>'
```
We see the main function compiles to just `return 50`:
```
 7cc:	d503201f 	nop

00000000000007d0 <frame_dummy>:
 7d0:	17ffffdc 	b	740 <register_tm_clones>

00000000000007d4 <main>:
 7d4:	52800640 	mov	w0, #0x32                  	// #50
 7d8:	d65f03c0 	ret

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
