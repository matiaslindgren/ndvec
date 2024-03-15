#include "ndvec.hpp"

int main() {
  ndvec v1(1, -2, 3);
  ndvec v2(-3, 2, -1);
  return (v1 - v2).sum() * v1.distance(v2);
}
