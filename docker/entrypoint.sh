#!/bin/bash

#Creating directory for root user SSH configuration
mkdir -p /root/.ssh/

#Defining keys to known hosts explicitly
ssh-keyscan github.com >> /root/.ssh/known_hosts
ssh-keyscan gitlab.com >> /root/.ssh/known_hosts

#If the variables for the test environment are defined, then we'll define the keys for the DUT and TREX servers too
if  [ -n "$JUMP_SERVER_IP" ] && [ -n "$JUMP_SERVER_USERNAME" ] && [ -n "$JUMP_SERVER_PORT" ] && \
    [ -n "$DUT_SERVER_IP" ] && [ -n "$TREX_SERVER_IP" ]; then
    ssh-keyscan -p ${JUMP_SERVER_PORT} ${JUMP_SERVER_IP} >> /root/.ssh/known_hosts

    JUMP_SERVER_SSH_CMD="ssh ${JUMP_SERVER_USERNAME}@${JUMP_SERVER_IP} -p ${JUMP_SERVER_PORT}"
    $JUMP_SERVER_SSH_CMD ssh-keyscan ${DUT_SERVER_IP} >> /root/.ssh/known_hosts
    $JUMP_SERVER_SSH_CMD ssh-keyscan ${TREX_SERVER_IP} >> /root/.ssh/known_hosts
fi

#If the username is no the root user, then we'll copy the configuration for the user too
if [ $USERNAME != "root" ]; then
    #SSH
    mkdir -p /home/$USERNAME/.ssh
    cp /root/.ssh/known_hosts /home/$USERNAME/.ssh/known_hosts
    chown -R $USERNAME:$USERNAME /home/$USERNAME/.ssh
fi

exec $@