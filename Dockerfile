FROM debian:bullseye

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
  build-essential \
  gcc \
  make \
  gdb \
  valgrind \
  strace \
  lsof \
  procps \
  file \
  vim \
  ca-certificates \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /malloc

CMD ["/bin/bash"]
