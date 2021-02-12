FROM ubuntu:20.04

ARG UNAME=oai-spgwu

WORKDIR /tmp/$UNAME

RUN apt-get update && \
      apt-get -y install sudo git

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=America/Sao_Pauloa
RUN apt-get update && \
      apt-get install -y tzdata

RUN apt-get update && \
      apt-get -y install cmake

RUN apt-get update && \
      apt-get -y install gcc g++

RUN apt-get update && \
      apt-get -y install libconfig++

RUN apt-get update && \
      apt-get -y install iproute2

RUN apt-get update && \
      apt-get -y install iptables

RUN apt-get update && \
      apt-get -y install iputils-ping

RUN apt-get update && \
      apt-get -y install wget

RUN apt-get update \
      # Install Docker CE CLI
      # The idea is to run docker network connect inside the container
      && apt-get install -y apt-transport-https ca-certificates curl gnupg2 lsb-release \
      && curl -fsSL https://download.docker.com/linux/$(lsb_release -is | tr '[:upper:]' '[:lower:]')/gpg | apt-key add - 2>/dev/null \
      && echo "deb [arch=amd64] https://download.docker.com/linux/$(lsb_release -is | tr '[:upper:]' '[:lower:]') $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list \
      && apt-get update \
      && apt-get install -y docker-ce-cli \
      # Install Docker Compose
      && LATEST_COMPOSE_VERSION=$(curl -sSL "https://api.github.com/repos/docker/compose/releases/latest" | grep -o -P '(?<="tag_name": ").+(?=")') \
      && curl -sSL "https://github.com/docker/compose/releases/download/${LATEST_COMPOSE_VERSION}/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose \
      && chmod +x /usr/local/bin/docker-compose

RUN apt-get update && \
      apt-get -y install tcpdump

RUN apt-get update && \
      apt-get -y install libelf-dev zlib1g clang llvm libasan5

# TODO navarrothiago - CO-RE
RUN apt-get update && \
      apt-get -y install linux-tools-common linux-headers-generic

RUN apt-get update && \
      apt-get -y install g++-multilib libc6-dev-i386

RUN apt-get update && \
      apt-get -y install ethtool

RUN apt-get update && \
      apt-get -y install gdb

RUN apt-get update && \
    apt-get install  -y \
    bash-completion

# It is comment due to ubuntu 20.04 base image.
# If you use another one, uncomment the lines below.
# RUN apt-get update && \
#       apt-get -y install libssl-dev && \
#       wget -c  https://github.com/Kitware/CMake/releases/download/v3.19.2/cmake-3.19.2.tar.gz -O - | tar -xz && \
#       cd cmake-3.19.2 && ./bootstrap && make && make install

CMD /bin/bash
