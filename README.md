# 5G UPF using eBPF/XDP

An open source C++ library powered by eBPF/XDP for user plane in the mobile core network (5G/LTE).

The key pillars of this project are:

- In-kernel fast packet processing
- Flexible and programmable dataplane
- Portable to different systems

These points are achieved mainly by eBPF/XDP and CO-RE (Compile Once - Run Everywhere) technologies.

This project is based on the following 3GPP Technical Specification:
- LTE; 5G; Interface between the Control Plane and the User Plane nodes (3GPP TS 29.244 version 16.5.0 Release 16)
- 5G; System architecture for the 5G System (5GS) (3GPP TS 23.501 version 16.5.0 Release 16)

The main goal is to enable in-kernel fast packet processing in third-party UPF/5G or SPGWu/LTE components in order to:
1. Boost them for those which does not have any fast packet processing enabled, or
2. Co-locate them with other fast packet processing solutions (e.g. DPDK)

Possible scenarios that take advantage of this type of technology: MEC, 5G NPN (Non Public Networks), on-premise, 5G enterprise, and much more.

## Design

The library is divided in layers:
- **Management Layer**: An user space layer responsible to receive requests from the third-party UPF/SPGWu components to manage PFCP sessions and eBPF programs lifecycle
- **Datapath Layer**: A kernel space layer representing by eBPF/XDP programs responsible to handle the user traffic (datapath) for fast packet processing

The high level design is shown in figure below.

<img src="img/up-ebpf-xdp-high-level.svg" alt="drawing" width="500"/>

The library has a component, called `PFCP Session Manager`, which is a C++ API responsible for managing PFCP (Packet Forwarding Control Protocol) sessions. For each session, there is an eBPF program that represents the PFCP context in the fast path. These programs are managed by the `eBPF Program Manager` component. The fast path is composed of three main functions: parser, traffic classifier and traffic forwarder. The image below shows this in more detail.

<img src="img/up-ebpf-xdp-high-level2.svg" alt="drawing" width="500"/>

A low-level design (Datapath Layer) is shown below.

<img src="img/up-ebpf-xdp-low-level.svg" alt="drawing" width="500"/>

## Features

As described in 3GPP TS 29.244, the Information Elements (IEs) are part of the PFCP context. The PFCP context is created by sending a PFCP Session Establishment Request message. The main features supported in this project are:

Management Layer - CRUD
- PFCP Session
- PDR (Packet Detection Rule)
- FAR (Forwarding Action Rule)

Fast Datapath Layer
- UDP and GTP parse
- Traffic classification based on PDR
- Traffic forwarding based on FAR

The logical data model between PFCP Session and IEs is shown in the image below. For more detail, see 3GPP TS 29.244 version 16.5.0 Release 16.

<img src="img/up-ebpf-xdp-ies.svg" alt="drawing" width="600"/>

## Future Work

- QER (QoS Enforcement Rule)
- CO-RE
- PoC with OpenAirInterface

## Main Dependencies

Core
- libbpf
- bpftool
- spdlog
- clang >= version 3.4.0
- llvm >= version 3.7.1
- kernel-headers => version 5.3
- cmake >= 3.16

Test
- scapy v2.4.3
- gtest
- sysstat
- trex v2.86

## First Steps

First of all, make sure you have installed [git-lfs](https://git-lfs.github.com/). The LFS repository is used to store the `bpftool` binary.

After downloaded and installed it, clone this repository:

```
git clone https://github.com/navarrothiago/upf-bpf.git
```

After cloning the repository, configure your [env.sh](env.sh) file (on the repository root folder) to match your dev or test environment, using the [.env.sample.sh](.env.sample.sh) file as a template

The project uses a docker container to build the UPF library. The command below will provision the docker image with all the project dependencies.

> :memo: You'll need the Docker Container Runtime package and the Docker Compose utility to set up the dev or test environment

```
make docker-build
```

After that, run the container with:

```
make docker-run
```

> You can also use the vscode development container feature to build the image and login into the container.
> Check [here](https://code.visualstudio.com/docs/remote/create-dev-container#_create-a-devcontainerjson-file) to understand how to open the [devcontainer.json](.devcontainer/devcontainer.json) file.

Inside the container, compile the dependencies with

```
make setup
```

## Build and Installation

The library is built and installed with

```
make install
```

The `package` folder is create with the headers, library and some binaries for testing.

```
package
├── bin       # Contains binaries for testing
├── include   # Contains headers
├── lib       # Contains libupf_xdp.a library
└── tests     # Contains scripts for testing
```

## How to Test

The test is based on RFC2544-like measurements. The testbed is composed of two servers: Trex Traffic Generator and HTTP API (upf-bpf). Both machines have Ubuntu 20.04.02 LTS installed with Linux kernel v5.4.0-72-generic. One machine is used to generate user traffic with TRex Traffic Generator and the other is the DUT (Device Under Test) where the upf-bpf is deployed. The setup is shown below.

Test environment:

<img src="img/setup-performance-evaluation.svg" alt="drawing" width="500"/>

### Setup Trex Traffic Generator Server

This machine must have installed the Trex traffic generator. You can check the [trex manual](https://trex-tgn.cisco.com/trex/doc/trex_manual.html) or you can based on the scripts that are called when `make trex` is executed (unstable).

### Setup HTTP API + upf-bpf Server

After running `make install` inside the docker container, copy the application `./package/bin/api` to the DUT machine. **Your host must have kernel >= v5.4**.

Steps:
1. Run Trex Traffic Generator
1. Run HTTP API + upf-bpf
1. Configure interfaces (/configure)
1. Create PFCP Session context (/createSession)
1. Configure the number of Rx queue in DUT
1. Generate the GTP/UDP flows (pkt size = 64B)
1. Collects metrics (CPU load, packet loss, throughput)

> :memo: Postman files are available: [Uplink](tests/api/requests_body/gtp-postman-colletion.json) and [Downlink](tests/api/requests_body/udp-postman-colletion.json). You will find the JSON messages used in step 3 and 4.

The flows are generated using [Trex Field Engine](https://trex-tgn.cisco.com/trex/doc/cp_stl_docs/api/field_engine.html). Check the implementation in the [run.py](tests/trex/test_cases/run.py) script. This script automates the steps 5, 6 and 7. It also generates a report with the metrics.

<img src="img/screenshot-tmux.png" alt="drawing" width="600"/>

> :memo: The tmux session is available [here](tests/scripts/start_session). There are still some parameters hardcoded. Feel free to change according to your needs. If you need any help, open an issue or contact me. PR are welcome!!

> :warning: Some scripts were developed to work in one environment. As you can see in [.env.sample.sh](.env.sample.sh), there are variables to configure the jump server, trex version, GTP and UDP interfaces (downlink and uplink), etc. **You might face some problems when trying to execute some of them, because they were not exhaustive tests in other environments.**

There are also UTs for Session Management layers. You can execute with (inside the container).

```
make config-veth-pair
make build-tests
make run-session-manager-tests
```
If you face any problem, feel free to open an issue or contact me.

## :rocket: Benchmark

Downlink | Uplink
---|---
<img src="tests/reports/img/Downlink Max Throughput.svg" alt="drawing" width="500"/>| <img src="tests/reports/img/Uplink Max Throughput.svg" alt="drawing" width="500"/>
<img src="tests/reports/img/Downlink Max ThroughputLoad per Core - 6 Rx Queue.svg" alt="drawing" width="500"/>| <img src="tests/reports/img/Uplink Max ThroughputLoad per Core - 6 Rx Queue.svg" alt="drawing" width="500"/>

Check the [Jupyter notebook](notebook.ipynb) to see how the graphics are generated.

> :memo: For more graphics, check [this](tests/reports/img) folder.

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

## TRex

If you faced the problem below, create a symbolic from `libc.a -> liblibc.a`
`No such file or directory: b'liblibc.a'`

## Contribute

If you think this could be better, **please open an issue or start a discussion**.

PRs ARE WELCOME :+1:!!

## BibTeX

```
@INPROCEEDINGS{Amar2110:Kernel,
AUTHOR="Thiago Arruda Navarro do Amaral and Raphael {Vicente Rosa} and David Moura
and Christian {Esteve Rothenberg}",
TITLE="An {In-Kernel} Solution Based on {XDP} for {5G} {UPF:} Design, Prototype
and Performance Evaluation",
BOOKTITLE="2021 1st Joint International Workshop on Network Programmability and
Automation (NetPA 2021)",
ADDRESS="Izmir, Turkey, Turkey",
DAYS=24,
MONTH=oct,
YEAR=2021,
KEYWORDS="5G; XDP; UPF",
ABSTRACT="The edge computing infrastructure can scale from datacenters to single
device. The well-known technology for fast packet processing is DPDK, which
has outstanding performance regarding the throughput and latency. However,
there are some drawbacks when the usage is done in the edge: (i) the
polling mechanism for packet processing keeps the CPU exclusively occupied
even if there is no traffic, leading to wasted resources; and (ii) DPDK
interface becomes unavailable for the applications inside the host, so the
integration between a non-DPDK application and a DPDK application becomes a
hard task. In this paper, we propose an open-source in-kernel 5G UPF
solution based on 3GPP Release 16 to be deployed in a restrictive
environment like MEC, where MEC host and UPF are collocated with the Base
Station, sharing the same computational and network resources. The solution
leverages the eBPF/XDP, a novel Linux kernel technology for fast packet
processing. We show it can scale and achieve 10 Mpps using only 60\% of the
CPU with 6 cores."
}
```

## Contact

- [Discord Server](https://discord.gg/TtdUvnA4nq)
- Mail: <navarro (dot) ime (at) gmail [dot] com>
- GitHub: [@navarrothiago](https://github.com/navarrothiago/)
- Twitter: [@navarr0thiag0](https://twitter.com/navarr0thiag0)

## References

- [Paper - An In-Kernel Solution Based on XDP for 5G UPF: Design, Prototype and Performance Evaluation](docs/Paper___An_In_Kernel_Solution_Based_on_XDP__Design__Prototype_and_Performance_Evaluation.pdf)
- [Video - Project Overview](https://youtu.be/Av_k_fZKCfM)
- [Slides - Project Overview](https://docs.google.com/presentation/d/1osOheCFV3c3wn4hDbo5R3coL8nDeaxjVb7D33QT41jw/edit?usp=sharing)
- [Video - Performance Evaluation with UPF eBPF/XDP Library for 5G Core Network](https://www.youtube.com/watch?v=6KYFDMJJH2o)
