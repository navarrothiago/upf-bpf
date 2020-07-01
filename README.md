User plane function using BPF for mobile network (4G/5G).

## Usage

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
- INTRIG IRC: https://discord.gg/ZcBHze8
