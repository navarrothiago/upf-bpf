# import stl_path
from trex_stl_lib.api import *

import time
import json
import argparse

# simple packet creation


def create_pkt(packet_len):
    base_pkt = Ether()/IP(src="16.0.0.1", dst="10.1.3.27")/UDP(dport=1234)
    pad = max(0, packet_len - len(base_pkt)) * 'x'

    vm = STLVM()

    # create a tuple var
    vm.tuple_var(name="tuple", ip_min="16.0.0.1", ip_max="16.0.0.254",
                 port_min=1234, port_max=1234, limit_flows=1000)

    # write fields
    vm.write(fv_name="tuple.ip", pkt_offset="IP.src")
    vm.fix_chksum()

    vm.write(fv_name="tuple.port", pkt_offset="UDP.sport")

    return STLPktBuilder(pkt=base_pkt/pad, vm=vm)

def simple_burst(packet_len=None, m=None):

    # create client
    c = STLClient()
    # username/server can be changed those are the default
    # username = common.get_current_user(),
    # server = "localhost"
    # STLClient(server = "my_server",username ="trex_client") for example
    passed = True

    try:
        # turn this on for some information
        # c.set_verbose("debug")

        # create one streams
        s1 = STLStream(packet=create_pkt(packet_len),
                       mode=STLTXCont())

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
        print("Running " + m + " on ports 0 for 10 seconds...")
        c.start(ports=[0], mult=m, duration=20)

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
parser.add_argument('-l',
                    '--packet_len',
                    type=int,
                    default=64,
                    help="The packets length in the stream")
parser.add_argument('-m',
                    '--multiplier',
                    default='100%',
                    help="The throughput in mpps on port 0 (e.g. 14mpps)")
args = parser.parse_args()

# Run the tests
simple_burst(packet_len=args.packet_len, m=args.multiplier)
