#!/bin/python3

# import stl_path
from trex_stl_lib.api import *

import time
import json
import argparse

import subprocess


def create_pkt_flow(size, nflows):
    print("{} flow will be generated...".format(nflows))
    base_pkt = Ether()/IP(src="16.0.0.1", dst="10.1.3.27")/UDP(dport=1234)
    pad = max(0, size - len(base_pkt)) * 'x'

    vm = STLVM()

    # create a tuple var
    vm.tuple_var(name="tuple", ip_min="16.0.0.1", ip_max="16.0.0.254",
                 port_min=1234, port_max=1234, limit_flows=nflows)

    # write fields
    vm.write(fv_name="tuple.ip", pkt_offset="IP.src")
    vm.fix_chksum()

    vm.write(fv_name="tuple.port", pkt_offset="UDP.sport")

    return STLPktBuilder(pkt=base_pkt/pad, vm=vm)


def create_pkt(size):
    # simple packet creation
    print("Only one flow will be generated...")
    pkt = Ether()/IP(src="10.1.2.27", dst="10.1.3.27")/UDP(dport=1234)
    pad = max(0, size - len(pkt)) * 'x'
    return STLPktBuilder(pkt=pkt/pad)


def simple_burst(s1=None, m=None, duration=None):

    # create client
    # c = STLClient()
    # username/server can be changed those are the default
    # username = common.get_current_user(),
    # server = "localhost"
    c = STLClient(server="localhost", sync_port=1235, async_port=1236)
    passed = True

    try:
        # turn this on for some information
        # c.set_verbose("debug")

        # create one streams
        # s1 = STLStream(packet=create_pkt(size, nflows), mode=STLTXCont())

        # connect to server
        c.connect()

        # prepare our ports (my machine has 0 <--> 1 with static route)
        # Acquire port 0 for $USER
        c.reset(ports=[0, 1])

        # add both streams to ports
        c.add_streams([s1], ports=[0])

        # clear the stats before injecting
        c.clear_stats()

        # set port 1 as promiscuous mode
        c.set_port_attr(ports=[1], promiscuous=True)

        # choose rate and start traffic for 10 seconds on 5 mpps
        print("Running " + m + " on ports 0 for {} seconds...".format(duration))
        c.start(ports=[0], mult=m, duration=duration)

        # block until done
        c.wait_on_traffic(ports=[0])

        # read the stats after the test
        stats = c.get_stats()

        print(json.dumps(stats[0], indent=4,
                         separators=(',', ': '), sort_keys=True))
        print(json.dumps(stats[1], indent=4,
                         separators=(',', ': '), sort_keys=True))

        print("\n")
        print(
            "Packets sent       0 --> 1:   {0} pkts".format(stats[0]["opackets"]))
        print(
            "Rx Mpps            0 --> 1:   {0} Mpps".format(float(stats[1]["rx_pps"])/1000000))

        if (stats[0]["opackets"] > 100):
            passed = True
        else:
            passed = False

    except STLError as e:
        passed = False
        print(e)

    finally:
        c.disconnect()

    if passed:
        print("\nTest has passed :-)\n")
    else:
        print("\nTest has failed :-(\n")


# Parse the args.
parser = argparse.ArgumentParser()
parser.add_argument('-s',
                    '--size',
                    type=int,
                    default=64,
                    help="The packets length in the stream")
parser.add_argument('-m',
                    '--multiplier',
                    default='100%',
                    help="The throughput in mpps on port 0 (e.g. 14mpps, 90%, 1kbps")
parser.add_argument('-d',
                    '--duration',
                    type=int,
                    default='10',
                    help="The duration of the transmission in second")
parser.add_argument('-f',
                    '--flows',
                    type=int,
                    default='6',
                    help="The number of flows. It must be equal or greater than 6")
args = parser.parse_args()

output = os.popen(
    'ssh india mpstat -P ALL {} 1 -o JSON'.format(args.duration)).read()
print(output)
# x_core = np.arange(1, 13, 1)

# Run the tests
simple_burst(s1=STLStream(packet=create_pkt_flow(args.size, args.flows), mode=STLTXCont()),
             m=args.multiplier,
             duration=args.duration)

# simple_burst(s1=STLStream(packet=create_pkt(args.size), mode=STLTXCont()),
#              m=args.multiplier,
#              duration=args.duration)
