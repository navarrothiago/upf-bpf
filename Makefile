SHELL := /bin/bash
GTP_INTERFACE?=$(GTP_INTERFACE)
UDP_INTERFACE?=$(UDP_INTERFACE)

.PHONY: help

help:
	@echo "Usage: make [comando]"
	@echo
	@echo "Comandos:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "- \033[36m%-30s\033[0m %s\n", $$1, $$2}'

# TODO navarrothiago - if you put "-j" or "--parallel" the in the make, a race condition occur.
# The problem is due to the copy object artifacts which occurs before the compilation. Create an dependece.
all: ## Build all
	cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_DEBUG_POSTFIX=d -DCMAKE_INSTALL_PREFIX="`pwd`/package" && \
  cmake --build build --target all --parallel && \
  cmake --build build --target copy_bpf_program  --parallel && \
  cmake --build build --target copy_samples_objs --parallel

install: ## Install package
	cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_DEBUG_POSTFIX=d -DCMAKE_INSTALL_PREFIX="`pwd`/package" && \
  cmake --build build --target install --parallel

setup: ## Build dependencies
	git submodule update --init --recursive && \
	cmake -Hextern/spdlog -Bextern/spdlog/build && \
  cmake --build extern/spdlog/build --target install --parallel && \
	cd extern/cpp-httplib/ && python3 split.py && cd - &&\
	cmake -Hextern/cpp-httplib -Bextern/cpp-httplib/build && \
  cmake --build extern/cpp-httplib/build --target install --parallel && \
	cmake -Hextern/json -Bextern/json/build -DJSON_MultipleHeaders=ON && \
  cmake --build extern/json/build --target install && \
	cd extern/libbpf/src && \
	make -j

rebuild: clean deload all ## Clean, deload and build all

clean: ## Clean all build files
	rm -Rf build

clean-all: clean ## Clean all build and dependencies
	cd extern/libbpf/src && \
	make clean
	rm -Rf extern/spdlog/build
	rm -Rf extern/cpp-httplib/build
	rm -Rf extern/json/build

all-verbose: ## Build all in verbose mode
	mkdir -p build && \
	cd build && \
	cmake ../ && \
	make V=1 && \
	make copy_bpf_program && \
	make copy_samples_objs && \
	make copy_objs V=1

config-veth-pair: ## Config veth pair. It must be run before <run-*> targets
	sudo bash ./tests/scripts/config_veth_pair

rm-veth-pair: ## Remove veth pairs
	sudo ip netns del ns0
	sudo ip netns del ns1

build-samples: ## Build samples
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
	./tests/scripts/run_sample xdp_hello_world

run-redirect-map-sample: all ## Build all and run BPF XDP redirect sample
	./tests/scripts/run_sample xdp_redirect_map

run-control-plane-tests: force-xdp-deload ## Run ControlPlaneTests
	./tests/scripts/run_test ControlPlaneTests

run-session-manager-tests: force-xdp-deload ## Run SessionManagerTests
	sudo -E bash ./tests/scripts/run_test UPFProgramTests $(GTP_INTERFACE) $(UDP_INTERFACE)

rerun: force-xdp-deload run ## Build all and run BPF XDP UPF

dut-run: ## Run ControlPlaneTests on DUT
	sudo ./bin/ControlPlaneTests

force-xdp-deload: ## Kill all and force deload XDP programs
	sudo -E bash tests/scripts/force_xdp_deload $(GTP_INTERFACE) $(UDP_INTERFACE)

trex: ## Install, deploy configuration and run t-rex on remote server
	tests/scripts/install_trex_remote
	tests/scripts/deploy_trex_config
	tests/scripts/run_trex_server

# TODO navarrothiago: wrap in a script.
api: force-xdp-deload ## Run server API
	build/tests/api/api 127.0.0.1 1234

trex-run-downlink-test-case: ## Run trex test case
	tests/scripts/run_test_case udp_downlink_tuple_gen

tmux: ## Create a test session using tmux
	tests/scripts/start_session

docker-build: ## Build development image
	docker/build_docker

docker-build-ubuntu-18.04: ## Build development image
	docker/build_docker ubuntu:18.04

docker-run: ## Run development container
	docker/run_docker

copy-control-plane-test: ## Copy to remote server ControlPlaneTest binary
	scp package/bin/ControlPlaneTests india:~/package/bin/

deploy-package: ## Deploy package (bin, lib, scripts) on the DUT server
	# ssh "${TREX_CLIENT_SSH}" pkill "${API_PROGRAM_NAME}"
	rsync -av package/ india:~/package

bpf: ## Compile bpf code
	cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_DEBUG_POSTFIX=d -DCMAKE_INSTALL_PREFIX="`pwd`/package" && \
  cmake --build build --target upf_xdp_bpf --parallel && \
  cmake --build build --target session_bpf --parallel && \
  cmake --build build --target far_bpf --parallel
