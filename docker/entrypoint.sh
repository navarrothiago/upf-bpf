#!/usr/bin/env bash

#Global Variables
SECRETS_FOLDER="/var/run/secrets"

#Function to create the user defined in the USERNAME Env
CreateUser() {
    useradd $USERNAME --uid 1000 --shell /bin/bash -G sudo \
    --create-home --home ${USERHOME}/ --user-group
    passwd -d $USERNAME
}

#Function to copy Secrets files to user home directory
ConfigureSSH() {
    if [ ! -d "${SSH_HOME_DIR}" ]; then
        mkdir -p "${SSH_HOME_DIR}"
    fi

    #Defining keys to known hosts explicitly
    ssh-keyscan github.com >> ${SSH_HOME_DIR}/known_hosts
    ssh-keyscan gitlab.com >> ${SSH_HOME_DIR}/known_hosts

    #If the variables for the SSH jump server are defined, then we'll get the host keys from the DUT
    # and TRex servers to it.
    if  [ -n "$JUMP_SERVER_IP" ] && [ -n "$JUMP_SERVER_USERNAME" ] && [ -n "$JUMP_SERVER_PORT" ] && \
        [ -n "$DUT_SERVER_IP" ] && [ -n "$TREX_SERVER_IP" ]; then
        ssh-keyscan -p ${JUMP_SERVER_PORT} ${JUMP_SERVER_IP} >> ${SSH_HOME_DIR}/known_hosts

        JUMP_SERVER_SSH_CMD="ssh ${JUMP_SERVER_USERNAME}@${JUMP_SERVER_IP} -p ${JUMP_SERVER_PORT}"
        $JUMP_SERVER_SSH_CMD ssh-keyscan ${DUT_SERVER_IP} >> ${SSH_HOME_DIR}/known_hosts
        $JUMP_SERVER_SSH_CMD ssh-keyscan ${TREX_SERVER_IP} >> ${SSH_HOME_DIR}/known_hosts
    fi

    #If the secrets are defined, them they will be copied to the root user ssh folder
    # so we can set the correct permissions and ownership
    if [ -f ${SECRETS_FOLDER}/SSH_PRIV_KEY ]; then 
        cp ${SECRETS_FOLDER}/SSH_PRIV_KEY "${SSH_HOME_DIR}"/${SSH_PRIVATE_KEY_FILE} && \
        export SSH_PRIVATE_KEY_FILE="${SSH_HOME_DIR}/${SSH_PRIVATE_KEY_FILE}"
    fi
    if [ -f ${SECRETS_FOLDER}/SSH_PUBL_KEY ]; then
        cp ${SECRETS_FOLDER}/SSH_PUBL_KEY "${SSH_HOME_DIR}"/${SSH_PUBLIC_KEY_FILE} && \
        export SSH_PUBLIC_KEY_FILE="${SSH_HOME_DIR}/${SSH_PUBLIC_KEY_FILE}"
    fi
    if [ -f ${SECRETS_FOLDER}/SSH_CONF_FILE ]; then
        cp ${SECRETS_FOLDER}/SSH_CONF_FILE "${SSH_HOME_DIR}/${SSH_CONFIG_FILE}" && \
        export SSH_CONFIG_FILE="${SSH_HOME_DIR}/${SSH_CONFIG_FILE}"
    fi

    #Correcting permissions
    chown -R ${USERNAME}:${USERNAME} "${USERHOME}"
    chmod -R 770 ${USERHOME}
    chmod -R 600 ${SSH_HOME_DIR}/*
}

#Function to configure the .gitconfig file for the user
ConfigureGit() {
    if [ -f ${SECRETS_FOLDER}/GIT_CONFIG_FILE ]; then
        cp ${SECRETS_FOLDER}/GIT_CONFIG_FILE ${USERHOME}/$(basename ${GIT_CONFIG}) && \
        export GIT_CONFIG="${USERHOME}/$(basename ${GIT_CONFIG})"
    fi
}

#Function to configure the .bashrc file for the user
ConfigureBashRC() {
    if [ -f ${SECRETS_FOLDER}/BASH_RC_FILE ]; then
        cp ${SECRETS_FOLDER}/BASH_RC_FILE ${USERHOME}/$(basename ${BASH_RC}) && \
        export BASH_RC="${USERHOME}/$(basename ${BASH_RC})"
    fi
}

#Defining user context configuration
if [ -n "$USERNAME" ] && [ $USERNAME != "root" ]; then
    USERHOME="/home/$USERNAME"
    SSH_HOME_DIR="${USERHOME}/.ssh"

    #Create the User inside the container
    CreateUser
else
    USERHOME="/root"
    SSH_HOME_DIR="${USERHOME}/.ssh"
fi

#Making Configurations
ConfigureSSH
ConfigureGit
ConfigureBashRC

#Entering the execution command as the user or as the root user
if [ -n "$USERNAME" ] && [ $USERNAME != "root" ]; then
    gosu $USERNAME "$@"
else
    exec "$@"
fi
