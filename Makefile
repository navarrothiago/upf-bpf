BPF_SAMPLES_DIR=build/samples
BPF_BINARY_DIR=build/tests
NUM_THREADS=4

# Get all PIDs from *xdp* that is running.
PIDS := $(shell ps -aux | grep -e UPFProgramTests -e xdp | awk '{print $$2}')

# TODO navarrothiago - Remove hardcoded.
# DEVICE_IN=wlp0s20f3
DEVICE_IN=eth0
DEVICE_OUT=veth0

.PHONY: help

help:
	@echo "Usage: make [comando]"
	@echo
	@echo "Comandos:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "- \033[36m%-30s\033[0m %s\n", $$1, $$2}'

# TODO navarrothiago - if you put the in the make, a race condition occur.
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
  cmake --build build --target install

setup: ## setup dependencies
	git submodule update --init --recursive && \
	cd extern/spdlog && \
	mkdir -p build && cd build && \
	cmake .. && make -j && sudo make install && \
	cd ../../../ && \
	cd extern/libbpf/src && \
	make -j


rebuild: clean deload all ## Clean, deload and build all

clean: ## Clean all build files
	rm -R build

clean-all: ## Clean all build and dependencies
	rm -R extern/spdlog/build
	rm -R build

all-verbose: ## Build all in verbose mode
	mkdir -p build && \
	cd build && \
	cmake ../ && \
	make V=1 && \
	make copy_bpf_program && \
	make copy_samples_objs && \
	make copy_objs V=1

config-veth-pair: ## Config veth pair. It must be run before <run-*> targets
	sudo ./tests/scripts/config_veth_pair.sh $(DEVICE_IN)

run-hello-world-samples: all ## Build all and run BPF XDP hello world sample
	cd $(BPF_SAMPLES_DIR) && \
	sudo ./xdp_hello_world | sudo cat /sys/kernel/debug/tracing/trace

run-redirect-map-sample: all ## Build all and run BPF XDP redirect sample
	pushd $(BPF_SAMPLES_DIR) && \
	sudo ./xdp_redirect_map -S $(DEVICE_IN) $(DEVICE_OUT)

run: ## Run BPF XDP UPF
	cd $(BPF_BINARY_DIR) && \
	sudo ./UPFProgramTests

rerun: force-xdp-deload run ## Build all and run BPF XDP UPF

run-scapy: ## Run scapy for packet manipulation
	sudo ./extern/scapy/run_scapy

force-xdp-deload: ## Kill all and force deload XDP programs
	sudo ip link set dev $(DEVICE_IN) xdpgeneric off
	sudo ip link set dev $(DEVICE_OUT) xdpgeneric off
	sudo kill -9 $(PIDS)


