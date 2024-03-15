FROM debian:stable-slim
ENV LANG=C.UTF-8
RUN apt update --yes \
    && apt install --yes curl \
    && curl --location https://apt.llvm.org/llvm-snapshot.gpg.key >> /etc/apt/trusted.gpg.d/apt.llvm.org.asc \
    && echo "deb http://apt.llvm.org/bookworm/ llvm-toolchain-bookworm-18 main" >> /etc/apt/sources.list \
    && echo "deb-src http://apt.llvm.org/bookworm/ llvm-toolchain-bookworm-18 main" >> /etc/apt/sources.list
RUN apt update --yes && apt install --yes \
      clang-18 \
      clang-format-18 \
      libc++-18-dev \
      libc++abi-18-dev \
      make
WORKDIR /ndvec
