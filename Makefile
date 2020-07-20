# diretório do projeto específico que contém o CMakeLists.txt
PROJECT_DIR=$(CURDIR)

BPF_SAMPLES_DIR=build/samples
BPF_BINARY_DIR=build/src
NUM_THREADS=4

.PHONY: help

help:
	@echo "Usage: make [comando]"
	@echo
	@echo "Comandos:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "- \033[36m%-30s\033[0m %s\n", $$1, $$2}'

all: ## Build all
	mkdir -p build && \
	pushd build && \
	cmake .. && \
	pushd ../extern/libbpf/src && \
	make && \
	popd && \
	make && \
	make copy_bpf_objs && \
	make copy_upf_xdp_bpf_objs

clean: ## Clean all build files
	rm -R build

all-verbose: ## Build all in verbose mode
	mkdir -p build && \
	pushd build && \
	cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON && \

run-samples: all ## Build all and run BPF XDP hello world sample
	pushd $(BPF_SAMPLES_DIR) && \
	sudo ./xdp_hello_world | sudo cat /sys/kernel/debug/tracing/trace

run: all ## Build all and run BPF XDP hello world sample
	pushd $(BPF_BINARY_DIR) && \
	sudo ./upf_xdp_user 

run-scapy: ## Run scapy for packet manipulation
	sudo ./extern/scapy/run_scapy
	