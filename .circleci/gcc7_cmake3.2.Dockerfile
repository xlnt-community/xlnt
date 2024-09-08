FROM gcc:7

# 1. Install CMake

ARG CMAKE_VERSION=3.2.0

WORKDIR /tmp/
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}.tar.gz
RUN tar -xzvf cmake-${CMAKE_VERSION}.tar.gz

WORKDIR /tmp/cmake-${CMAKE_VERSION}
RUN ./bootstrap
RUN make -j$(nproc --all)
RUN make install

# 2. Install required de_DE locale (for tests)

RUN apt-get update -y
RUN apt-get install -y --no-install-recommends locales
RUN sed -i '/^# de_DE /s/^# //' /etc/locale.gen
RUN locale-gen

# 3. Cleanup

WORKDIR /
RUN rm -rf /tmp/*
