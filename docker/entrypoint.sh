#!/usr/bin/env bash

#Creating directory for root user SSH configuration
SSH_ROOT_DIR=/root/.ssh/
mkdir -p "${SSH_ROOT_DIR}"

#Defining keys to known hosts explicitly
ssh-keyscan github.com >> /root/.ssh/known_hosts
ssh-keyscan gitlab.com >> /root/.ssh/known_hosts

#If the variables for the SSH jump server are defined, then we'll get the host keys from the DUT
# and TRex servers to it.
if  [ -n "$JUMP_SERVER_IP" ] && [ -n "$JUMP_SERVER_USERNAME" ] && [ -n "$JUMP_SERVER_PORT" ] && \
    [ -n "$DUT_SERVER_IP" ] && [ -n "$TREX_SERVER_IP" ]; then
    ssh-keyscan -p ${JUMP_SERVER_PORT} ${JUMP_SERVER_IP} >> /root/.ssh/known_hosts

    JUMP_SERVER_SSH_CMD="ssh ${JUMP_SERVER_USERNAME}@${JUMP_SERVER_IP} -p ${JUMP_SERVER_PORT}"
    $JUMP_SERVER_SSH_CMD ssh-keyscan ${DUT_SERVER_IP} >> /root/.ssh/known_hosts
    $JUMP_SERVER_SSH_CMD ssh-keyscan ${TREX_SERVER_IP} >> /root/.ssh/known_hosts
fi

#If the secrets are defined, them they will be copied to the root user ssh folder
# so we can set the correct permissions and ownership
SECRETS_FOLDER="/var/run/secrets"
if [ -f ${SECRETS_FOLDER}/SSH_PRIV_KEY ]; then cp ${SECRETS_FOLDER}/SSH_PRIV_KEY \
   "${SSH_ROOT_DIR}"/${SSH_PRIVATE_KEY_FILE}; fi
if [ -f ${SECRETS_FOLDER}/SSH_PUBL_KEY ]; then cp ${SECRETS_FOLDER}/SSH_PUBL_KEY \
    "${SSH_ROOT_DIR}"/${SSH_PUBLIC_KEY_FILE}; fi
if [ -f ${SECRETS_FOLDER}/SSH_CONF_FILE ]; then cp ${SECRETS_FOLDER}/SSH_CONF_FILE \
    "${SSH_ROOT_DIR}"/${SSH_CONFIG_FILE}; fi
if [ -f ${SECRETS_FOLDER}/BASH_RC_FILE ]; then cp ${SECRETS_FOLDER}/BASH_RC_FILE \
    /root ; fi
if [ -f ${SECRETS_FOLDER}/GIT_CONFIG_FILE ]; then cp ${SECRETS_FOLDER}/GIT_CONFIG_FILE \
    /root ; fi
chown -R root:root "${SSH_ROOT_DIR}"
chmod -R 600 "${SSH_ROOT_DIR}"

#If the username is not the root user, then we'll copy the configuration
# for the user too
if [ -n "$USERNAME" ] && [ $USERNAME != "root" ]; then
    USERHOME="/home/$USERNAME"

    #UNIX
    useradd $USERNAME --uid 1000 --shell /bin/bash -G sudo \
    --create-home --home ${USERHOME}/ --user-group
    passwd -d $USERNAME

    #SSH
    mkdir -p ${USERHOME}/.ssh
    cp ${SSH_FOLDER}/* ${USERHOME}/.ssh/

    #BASHRC
    if [ -f ${SECRETS_FOLDER}/BASH_RC_FILE ]; then cp ${BASH_RC} \
        ${USERHOME}/$(basename ${BASH_RC}); fi

    #GITCONFIG
    if [ -f ${SECRETS_FOLDER}/GIT_CONFIG_FILE ]; then cp ${GIT_CONFIG} \
        ${USERHOME}/$(basename ${GIT_CONFIG}); fi

    chown -R $USERNAME:$USERNAME ${USERHOME}
fi

#Entering the execution command as the user or as the root user
if [ -n "$USERNAME" ] && [ $USERNAME != "root" ]; then
    gosu $USERNAME "$@"
else
    exec "$@"
fi
