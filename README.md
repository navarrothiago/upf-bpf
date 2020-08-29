User plane function using BPF and XDP for mobile core network (4G/5G).

## Usage

### Testing

![setup-for-GPDU-debug-issue-2-upf-bpf](https://user-images.githubusercontent.com/42647168/86470179-4486cc80-bd11-11ea-8f55-fee848b12e11.png)

- Open terminal in `Linux #0`
- Configure veth pairs with `make config-veth-pair` in `Linux #0`
- Install the spdlog library with `make install`
- Build, load and run program with `make run` in `Linux #0`
- Generate GTP G-PDU using [Scapy](https://github.com/secdev/scapy) with `gtpu-traffic-generator.py`  in `Linux #1`
- Open other terminal tab in `Linux #0`
- Check logs: `sudo cat /sys/kernel/debug/tracing/trace | grep "GPDU"`  in `Linux #0`

### Make Commands

- all                            Build all
- clean                          Clean all build files
- all-verbose                    Build all in verbose mode
- config-veth-pair               Config veth pair. It must be run before `run-*` targets
- run-hello-world-samples        Build all and run BPF XDP hello world sample
- run-redirect-map-sample        Build all and run BPF XDP redirect sample
- run                            Build all and run BPF XDP UPF
- run-scapy                      Run scapy for packet manipulation
- force-xdp-deload               Force deload XDP programs

## Dependencies

- libbpf
- libelf
- libz
- clang >= version 3.4.0
- llvm >= version 3.7.1
- kernel-headers => version 5.3
- bpftool
- scapy v2.4.3
- spdlog
- gtest

## Tree

The directory structure was created based on this [notes](https://blogs.oracle.com/linux/notes-on-bpf-4).

```
├── build: Generated build directory.
├── cmake: Cmake files configuration directory
├── CMakeLists.txt: Cmake file
├── extern: Submodule repositories
├── include: Include files
├── LICENSE: File license
├── Makefile: Encapsulate cmake calls for build, run samples, clean, etc
├── README.md: Readme file
├── samples: Samples like XDP BPF hello world
└── src: Source files directory
```

## Contact

- :sunglasses:: Thiago Navarro
- :mailbox:: navarro.ime@gmail.com
- Discord: navarrothiago#9698
- INTRIG Discord: https://discord.gg/ZcBHze8

## Feel free to contribuite
