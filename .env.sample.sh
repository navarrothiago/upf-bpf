# Docker container workspace.
WORKSPACE=/workspaces

BPF_SAMPLES_DIR="${WORKSPACE}"/build/samples
BPF_BINARY_DIR="${WORKSPACE}"/build/tests

# Compilation environment variable.
NUM_THREADS=

# Docker environment variable.
USERNAME=oai-spgwu
IMAGE_TAG=upee
IMAGE_VERSION=v1.0
DOCKERFILE=Dockerfile
DOCKERCOMPOSEFILE=docker-compose.yml
SSH_FOLDER=~/.ssh
SSH_PUBLIC_KEY_FILE=id_rsa.pub
SSH_PRIVATE_KEY_FILE=id_rsa
SSH_CONFIG_FILE=config
GIT_CONFIG=~/.gitconfig
BASH_RC=~/.bashrc

# TODO navarrothiago - Remove hardcoded https://github.com/navarrothiago/upf-bpf/issues/24
DEVICE_IN=
DEVICE_OUT_UL=
DEVICE_OUT_DL=

# TODO navarrothiago - pass as exec param.
GTP_INTERFACE=
UDP_INTERFACE=
SOCKET_BUFFER_ENABLED=0

# Test environment variables.
TEST_CASE=hello_world
GTEST_FILTER_ARGS="*.*"

# Jump server.
JUMP_SERVER_NAME=
JUMP_SERVER_USERNAME=
JUMP_SERVER_IP=
JUMP_SERVER_PORT=

# Trex version.
TREX_VERSION=v2.87
TREX_SHA256SUM=290c1be468335a2de2e69f217b139c9b1198732e529bfd069348d05297548b8a

# Trex server configuration.
TREX_SERVER_NAME= # Warning: Optional - If you set the name, it must be configured on your ssh config.
TREX_SERVER_IP=
TREX_SERVER_ASYNC_PORT=
TREX_SERVER_SYNC_PORT=
TREX_SERVER_USERNAME=
TREX_SERVER_SSH="${TREX_SERVER_NAME}"
# TREX_SERVER_SSH="${TREX_SERVER_USERNAME}"@"${TREX_SERVER_IP}"
TREX_SERVER_UPLOAD_DIR=
TREX_SERVER_DIR="${TREX_SERVER_UPLOAD_DIR}"/"${TREX_VERSION}"

# Trex client configuration.
TREX_CLIENT_NAME= # Warning: Optional - If you set the name, it must be configured on your ssh config.
TREX_CLIENT_IP=
TREX_CLIENT_USERNAME=
TREX_CLIENT_SSH="${TREX_CLIENT_NAME}"
# TREX_CLIENT_SSH="${TREX_CLIENT_USERNAME}"@"${TREX_CLIENT_IP}"
TREX_CLIENT_UPLOAD_DIR=
TREX_CLIENT_DIR="${TREX_CLIENT_UPLOAD_DIR}"/trex_client
TREX_CLIENT_LIB_DIR="${TREX_CLIENT_DIR}"/interactive

# DUT - Device Under Test Configuration.
DUT_NAME= # Warning: Optional - If you set the name, it must be configured on your ssh config.
DUT_IP=
DUT_USERNAME=
DUT_UPLOAD_DIR=

# Test local configuration.
LOCAL_CONFIG_DIR="${WORKSPACE}"/tests/trex/config
LOCAL_TRAFFIC_DIR="${WORKSPACE}"/tests/trex/traffic
LOCAL_TEST_CASES_DIR="${WORKSPACE}"/tests/trex/test_cases
LOCAL_SERVER_UPLOAD_DIR="${WORKSPACE}"/tests/trex
LOCAL_PACKAGE="${WORKSPACE}"/package

# Test remote configuration
REMOTE_CONFIG_DIR="${TREX_SERVER_UPLOAD_DIR}"/config
REMOTE_TRAFFIC_DIR="${TREX_SERVER_UPLOAD_DIR}"/traffic
REMOTE_TEST_CASES_DIR="${TREX_SERVER_UPLOAD_DIR}"/test_cases

# SSH port forwarding configuration
LOCAL_HTTP_SSH_PORT_FORWARDING=
LOCAL_TREX_SYNC_SSH_PORT_FORWARDING=
LOCAL_TREX_ASYNC_SSH_PORT_FORWARDING=
API_HTTP_PORT=

# Programs name
API_PROGRAM_NAME=api

PYTHONPATH=/workspaces/tests/trex/trex_client/interactive/
