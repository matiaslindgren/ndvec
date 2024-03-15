#!/usr/bin/env sh
set -xue

docker build --tag ndvec:1 .

docker run \
  --pull never \
  --rm \
  -v ${PWD}/ndvec.hpp:/ndvec/ndvec.hpp \
  -v ${PWD}/main.cpp:/ndvec/main.cpp \
  -v ${PWD}/Makefile:/ndvec/Makefile \
  -v ${PWD}/.clang-format:/ndvec/.clang-format \
  --interactive \
  --tty \
  ndvec:1 \
  bash
