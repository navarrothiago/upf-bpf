FROM ubuntu:20.04

ARG GIT_TAG=2019.w47
ARG UID=1000
ARG GID=1000
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

RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 93C4A3FD7BB9C367 && \
      apt-get update && \
      apt-get -y install ansible

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

# Creates openair-cn-cups user. The password is the username.
#RUN useradd -m openair-cn-cups && echo "openair-cn-cups:openair-cn-cups" | chpasswd && adduser openair-cn-cups sudo

# The user and group `$UNAME` is created.
# The user `$UNAME` is added to the both the `$UNAME` and `sudo` group.
# The `uid` and `gid` is set to the value of `999`.
# The home directory is set to `/home/$UNAME`.
# The shell is set to `/bin/bash`.
# The `sed` command does inline updates to the `/etc/sudoers` file to allow `$UNAME` and `root` users passwordless access to the `sudo` group.
# The `sed` command disables the `#includedir` directive that would allow any files in subdirectories to override these inline updates.
# Ref: https://superuser.com/questions/1296533/sudo-inside-docker-on-fedora-ask-for-password-on-host-it-doesnt-ask-password/1491777#1491777
RUN echo "GUI=${GID} and UID=${UID}"

RUN groupadd -g ${GID} $UNAME && useradd -u ${UID} -g $UNAME -G sudo -m -s /bin/bash $UNAME && \
    sed -i /etc/sudoers -re 's/^%sudo.*/%sudo ALL=(ALL:ALL) NOPASSWD: ALL/g' && \
    sed -i /etc/sudoers -re 's/^root.*/root ALL=(ALL:ALL) NOPASSWD: ALL/g' && \
    sed -i /etc/sudoers -re 's/^#includedir.*/## **Removed the include directive** ##"/g' && \
    echo "$UNAME ALL=(ALL:ALL) NOPASSWD: ALL" >> /etc/sudoers && \
    echo "Customized the sudoers file for passwordless access to the $UNAME user!" && \
    echo "$UNAME user:";  su - $UNAME -c id

# Persiste bash history
# https://code.visualstudio.com/docs/remote/containers-advanced#_persist-bash-history-between-runs
# FIXME: Do not forget to remove \ in the end of the last line
RUN SNIPPET="export PROMPT_COMMAND='history -a' && export HISTFILE=/commandhistory/.bash_history" \
      && echo $SNIPPET >> "/root/.bashrc" \
      # [Optional] If you have a non-root user
      && mkdir /commandhistory \
      && touch /commandhistory/.bash_history \
      && chown -R $UNAME /commandhistory \
      && echo $SNIPPET >> "/home/$UNAME/.bashrc"

RUN apt-get update && \
      apt-get -y install tcpdump

RUN apt-get update && \
      apt-get -y install libelf-dev zlib1g clang llvm

# TODO navarrothiago - CO-RE
RUN apt-get update && \
      apt-get -y install linux-tools-common linux-headers-generic

RUN apt-get update && \
      apt-get -y install g++-multilib libc6-dev-i386

RUN apt-get update && \
      apt-get -y install ethtool

RUN apt-get update && \
      apt-get -y install gdb

# RUN apt-get update && \
#       apt-get -y install libssl-dev && \
#       wget -c  https://github.com/Kitware/CMake/releases/download/v3.19.2/cmake-3.19.2.tar.gz -O - | tar -xz && \
#       cd cmake-3.19.2 && ./bootstrap && make && make install

USER root
#USER $UNAME

CMD /bin/bash
