User plane function using BPF and XDP for mobile core network (4G/5G).

## Usage

### Testing

![setup-for-GPDU-debug-issue-2-upf-bpf](https://user-images.githubusercontent.com/42647168/86470179-4486cc80-bd11-11ea-8f55-fee848b12e11.png)

- Open terminal in `Linux #0` 
- Build, load and run program with `make run` in `Linux #0`
- Generate GTP G-PDU using [Scapy](https://github.com/secdev/scapy) with `gtpu-traffic-generator.py`  in `Linux #1`
- Open other terminal tab in `Linux #0`
- Run `sudo tcpdump -i <NIC1 INTEFACE>`, where `NIC1 INTERFACE` is the network interface
- Open other terminal tab in `Linux #0`
- Run `sudo cat /sys/kernel/debug/tracing/trace | grep "GPDU"`  in `Linux #0`
- Check if there is a message `GTP GPDU received`

### Make Commands

- ```make all``` :                            Build all
- ```make clean```:                           Clean all build files
- ```make all-verbose```:                     Build all in verbose mode
- ```make run-samples```:                     Build all and run XDP BPF sample

## Dependencies

- libbpf
- libelf
- libz
- clang >= version 3.4.0
- llvm >= version 3.7.1
- kernel-headers
- bpftool 
- scapy v2.4.3

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
- INTRIG Discord server: https://discord.gg/ZcBHze8

## Feel free to contribuite
