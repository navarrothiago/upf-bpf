# Testing

![setup-for-GPDU-debug-issue-2-upf-bpf](https://user-images.githubusercontent.com/42647168/86470179-4486cc80-bd11-11ea-8f55-fee848b12e11.png)

- Open terminal in `Linux #0`
- Configure veth pairs with `make config-veth-pair` in `Linux #0`
- Install the spdlog library with `make setup`
- Build, load and run program with `make run` in `Linux #0`
- Login remote Scapy `ssh -X navarro@192.168.15.7 "cd /work/mestrado/scapy/; sudo -S ./run_scapy"`
- Generate GTP G-PDU using [Scapy](https://github.com/secdev/scapy) with `gtpu-traffic-generator.py`  in `Linux #1`
- Open other terminal tab in `Linux #0`
- Check logs: `sudo cat /sys/kernel/debug/tracing/trace | grep "GPDU"`  in `Linux #0`