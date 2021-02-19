BPF_SAMPLES_DIR=build/samples
BPF_BINARY_DIR=build/tests
NUM_THREADS=4
GTEST_FILTER_ARGS="*.*"
USERNAME=oai-spgwu
IMAGE=upee
VERSION=v1.0
DOCKERFILE_DEVEL=Dockerfile
TEST_CASE?=hello_world


# Get all PIDs from *xdp* that is running.
PIDS := $(shell ps -aux | grep -e UPFProgramTests -e xdp | awk '{print $$2}')

# TODO navarrothiago - Remove hardcoded https://github.com/navarrothiago/upf-bpf/issues/24
# DEVICE_IN=wlp0s20f3
# Uncomment fo Docker standalone
# DEVICE_IN=eth0
# Uncomment for OAI
# DEVICE_IN=enp0s20f0u4u2u4
DEVICE_IN?=enp3s0f0
DEVICE_OUT_UL=veth0
DEVICE_OUT_DL=enp3s0f1

.PHONY: help

help:
	@echo "Usage: make [comando]"
	@echo
	@echo "Comandos:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "- \033[36m%-30s\033[0m %s\n", $$1, $$2}'

# TODO navarrothiago - if you put "-j" or "--parallel" the in the make, a race condition occur.
# The problem is due to the copy object artifacts which occurs before the compilation. Create an dependece.
all: ## Build all
	mkdir -p build && \
	cd build && \
	cmake ../ && \
	make all && \
	make copy_bpf_program && \
	make copy_samples_objs && \
	make copy_objs

install: ## Install package
	cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_DEBUG_POSTFIX=d -DCMAKE_INSTALL_PREFIX="`pwd`/package" && \
  cmake --build build --target install --parallel

setup: ## Build dependencies
	git submodule update --init --recursive && \
	cd extern/spdlog && \
	mkdir -p build && cd build && \
	cmake .. && make -j && sudo make install && \
	cd ../../../ && \
	cd extern/libbpf/src && \
	make -j

rebuild: clean deload all ## Clean, deload and build all

clean: ## Clean all build files
	rm -Rf build

clean-all: clean ## Clean all build and dependencies
	cd extern/libbpf/src && \
	make clean
	rm -Rf extern/spdlog/build

all-verbose: ## Build all in verbose mode
	mkdir -p build && \
	cd build && \
	cmake ../ && \
	make V=1 && \
	make copy_bpf_program && \
	make copy_samples_objs && \
	make copy_objs V=1

config-veth-pair: ## Config veth pair. It must be run before <run-*> targets
	sudo ./tests/scripts/config_veth_pair ns0 $(DEVICE_OUT_UL) $(DEVICE_IN)
	sudo ./tests/scripts/config_veth_pair ns1 $(DEVICE_OUT_DL) $(DEVICE_IN)

rm-veth-pair: ## Remove veth pairs
	sudo ip netns del ns0
	sudo ip netns del ns1

bild-samples: ## Build samples
	cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_DEBUG_POSTFIX=d -DCMAKE_INSTALL_PREFIX="`pwd`/package" && \
	cmake --build build --target xdp_hello_world &&  \
	cmake --build build --target xdp_redirect_map &&  \
	cmake --build build --target sockex3 && \
	cmake --build build --target copy_samples_objs

build-tests: ## Build tests
	cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_DEBUG_POSTFIX=d -DCMAKE_INSTALL_PREFIX="`pwd`/package" && \
	cmake --build build --target ControlPlaneTests -j &&  \
	cmake --build build --target UPFProgramTests -j && \
	cmake --build build --target copy_bpf_program

clean-samples: ## Clean samples artifacts
	rm -R build/samples

run-hello-world-samples: all ## Build all and run BPF XDP hello world sample
	cd $(BPF_SAMPLES_DIR) && \
	sudo ./xdp_hello_world | sudo cat /sys/kernel/debug/tracing/trace

run-redirect-map-sample: all ## Build all and run BPF XDP redirect sample
	pushd $(BPF_SAMPLES_DIR) && \
	sudo ./xdp_redirect_map -S $(DEVICE_IN) $(DEVICE_OUT_UL)

run-control-plane-tests: force-xdp-deload ## Run ControlPlaneTests
	cd $(BPF_BINARY_DIR) && \
	sudo ./ControlPlaneTests

run-session-manager-tests: force-xdp-deload ## Run SessionManagerTests
	cd $(BPF_BINARY_DIR) && \
	sudo ./UPFProgramTests --gtest_filter=$(GTEST_FILTER_ARGS)

rerun: force-xdp-deload run ## Build all and run BPF XDP UPF

dut-run: ## Run ControlPlaneTests on DUT
	sudo ./bin/ControlPlaneTests

force-xdp-deload: ## Kill all and force deload XDP programs
	# the "true" is used to avoid stop when execute the command.
	sudo ip link set dev $(DEVICE_IN) xdpgeneric off | true
	sudo ip link set dev $(DEVICE_OUT_UL) xdpgeneric off | true
	sudo ip link set dev $(DEVICE_OUT_DL) xdpgeneric off | true
	sudo kill -9 $(PIDS) | true

trex: ## Install, deploy configuration and run t-rex on remote server
	tests/scripts/install_trex_remote
	tests/scripts/deploy_trex_config
	tests/scripts/run_trex_server

trex-tests: ## Run trex test case
	tests/scripts/run_test_case $(TEST_CASE)

docker-build: ## Build development image
	docker build --tag=$(IMAGE):$(VERSION) --rm -f docker/$(DOCKERFILE_DEVEL) .

docker-run: ## Run development container
	docker run -it --rm \
		--volume $(PWD):/workspace \
		--volume $(HOME)/.ssh:/root/.ssh/ \
		--volume $(HOME)/.ssh:/home/$(USERNAME)/.ssh/ \
		--privileged \
		--workdir /workspace $(IMAGE):$(VERSION) \
		/bin/bash
