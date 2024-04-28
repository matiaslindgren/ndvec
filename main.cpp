#include "ndvec.hpp"

int main() {
  using ndvec::ndvec;
  ndvec v1(1, -2, 3);
  ndvec v2(-3, 2, -1);
  ndvec v3(5, 6, 3);
  ndvec v4(1, 2, -3, -2, 4, 10);
  return v4.signum().sum() + (v1 - v2).sum() * v1.distance(v3);
}
