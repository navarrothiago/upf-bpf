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

RUN apt-get update && \
    apt-get install  -y \
    tmux

# It is comment due to ubuntu 20.04 base image.
# If you use another one, uncomment the lines below.
# RUN apt-get update && \
#       apt-get -y install libssl-dev && \
#       wget -c  https://github.com/Kitware/CMake/releases/download/v3.19.2/cmake-3.19.2.tar.gz -O - | tar -xz && \
#       cd cmake-3.19.2 && ./bootstrap && make && make install

# Add your local bashrc
ADD .bashrc /home/$USERNAME/
ADD .bashrc /root/

# Make ssh dir
RUN mkdir /root/.ssh/

# Copy over private key, and set permissions
# WARNING! Anyone who gets their hands on this image will be able
# to retrieve this private key file from the corresponding image layer
ADD id_rsa /root/.ssh/id_rsa
ADD id_rsa.pub /root/.ssh/id_rsa.pub
ADD config /root/.ssh/config
ADD id_rsa /home/$USERNAME/.ssh/id_rsa
ADD id_rsa.pub /home/$USERNAME/.ssh/id_rsa.pub
ADD config /home/$USERNAME/.ssh/config

# Create known_hosts
# TODO navarrothiago - Put ssh-keyscan of the dut and trex server.
RUN touch /root/.ssh/known_hosts
RUN ssh-keyscan github.com >> /root/.ssh/known_hosts && \
    ssh-keyscan gitlab.com >> /root/.ssh/known_hosts &&\
    ssh-keyscan github.com >> /home/$USERNAME/.ssh/known_hosts &&\
    ssh-keyscan gitlab.com >> /home/$USERNAME/.ssh/known_hosts && \
    chown -R $USERNAME:$USERNAME /home/$USERNAME/.ssh 


ARG JUMP_SERVER_IP
ARG JUMP_SERVER_PORT
ARG JUMP_SERVER_USERNAME
ARG DUT_SERVER_IP
ARG TREX_SERVER_IP

RUN ssh-keyscan -p ${JUMP_SERVER_PORT} ${JUMP_SERVER_IP} >> /root/.ssh/known_hosts &&\
    ssh ${JUMP_SERVER_USERNAME}@${JUMP_SERVER_IP} -p ${JUMP_SERVER_PORT} ssh-keyscan ${DUT_SERVER_IP} >> /root/.ssh/known_hosts &&\
    ssh ${JUMP_SERVER_USERNAME}@${JUMP_SERVER_IP} -p ${JUMP_SERVER_PORT} ssh-keyscan ${TREX_SERVER_IP} >> /root/.ssh/known_hosts &&\
    ssh-keyscan -p ${JUMP_SERVER_PORT} ${JUMP_SERVER_IP} >> /home/$USERNAME/.ssh/known_hosts &&\
    ssh ${JUMP_SERVER_USERNAME}@${JUMP_SERVER_IP} -p ${JUMP_SERVER_PORT} ssh-keyscan ${DUT_SERVER_IP} >> /home/$USERNAME/.ssh/known_hosts &&\
    ssh ${JUMP_SERVER_USERNAME}@${JUMP_SERVER_IP} -p ${JUMP_SERVER_PORT} ssh-keyscan ${TREX_SERVER_IP} >> /home/$USERNAME/.ssh/known_hosts

# Locales for Yocto
ENV LANG=en_US.UTF-8
RUN apt-get install -y locales && \
    echo "$LANG UTF-8" > /etc/locale.gen && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=$LAN

# Install Docker CE CLI
RUN apt-get install -y apt-transport-https ca-certificates curl gnupg-agent software-properties-common lsb-release \
    && curl -fsSL https://download.docker.com/linux/$(lsb_release -is | tr '[:upper:]' '[:lower:]')/gpg | (OUT=$(apt-key add - 2>&1) || echo $OUT) \
    && add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/$(lsb_release -is | tr '[:upper:]' '[:lower:]') $(lsb_release -cs) stable" \
    && apt-get update \
    && apt-get install -y docker-ce-cli

RUN chown root /root/.ssh/config

RUN apt-get update && \
    apt-get install  -y \
    python3-pip

RUN pip3 install ipykernel
RUN pip3 install matplotlib
RUN pip3 install paramiko
RUN pip3 install autopep8

RUN apt-get update && \
    apt-get install  -y \
    rsync

ARG ENV_FILENAME="env.sh"
ADD ${ENV_FILENAME} ${ENV_FILENAME}

# Omit if you want to keep the default as root.
# USER $USERNAME

CMD ${ENV_FILENAME} && /bin/bash
