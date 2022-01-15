ARG BASE_DEV_IMAGE=ubuntu:20.04
FROM $BASE_DEV_IMAGE

#Configurations for the general environment
ARG UNAME=oai-spgwu
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=America/Sao_Paulo
ENV LANG=en_US.UTF-8

WORKDIR /tmp/$UNAME

#Install basic software to update repository list
RUN apt-get update && \
      apt-get install -y apt-transport-https ca-certificates curl gnupg-agent gnupg2 software-properties-common lsb-release && \
      curl -fsSL https://download.docker.com/linux/$(lsb_release -is | tr '[:upper:]' '[:lower:]')/gpg | apt-key add - 2>/dev/null && \
      add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/$(lsb_release -is | tr '[:upper:]' '[:lower:]') $(lsb_release -cs) stable"

#Install deps packages
RUN apt-get update && \
      apt-get -y install sudo tzdata git cmake gcc g++ libconfig++ iproute2 iptables iputils-ping wget \
      python3-pip locales ethtool docker-ce-cli libssl-dev rsync gdb bash-completion tmux \
      tcpdump libelf-dev zlib1g clang llvm libasan5 linux-tools-common linux-headers-generic \
      g++-multilib libc6-dev-i386 gosu

RUN pip3 install numpy

#Downloading Docker Compose
RUN   LATEST_COMPOSE_VERSION=$(curl -sSL "https://api.github.com/repos/docker/compose/releases/latest" | grep -o -P '(?<="tag_name": ").+(?=")') \
      && curl -sSL "https://github.com/docker/compose/releases/download/${LATEST_COMPOSE_VERSION}/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose \
      && chmod +x /usr/local/bin/docker-compose

# Ubuntu 20.04 already support cmake 3.19.
RUN if [ $BASE_DEV_IMAGE != "ubuntu:20.04" ];  then \
      wget -c  https://github.com/Kitware/CMake/releases/download/v3.19.2/cmake-3.19.2.tar.gz -O - | tar -xz && \
      cd cmake-3.19.2 && ./bootstrap && make -j4 && make install; fi

# Locales for Yocto
RUN echo "$LANG UTF-8" > /etc/locale.gen && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=$LAN

# Copy Entrypoint Script to Container
COPY entrypoint.sh /entrypoint.sh

CMD ["/bin/bash"]
ENTRYPOINT ["/entrypoint.sh"]
