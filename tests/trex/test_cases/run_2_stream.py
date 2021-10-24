#import stl_path
from trex_stl_lib.api import *

import time
import json

# simple packet creation
def create_pkt(src):

    return STLPktBuilder(
        pkt=Ether()/IP(src=src, dst="10.1.3.27") /
        UDP(dport=1234)/Raw('x'*20)
    )

def simple_burst():

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

        # create two streams
        s1 = STLStream(packet=create_pkt("10.1.2.29"),
                       mode=STLTXCont())
        s2 = STLStream(packet=create_pkt("10.1.2.30"),
                       mode=STLTXCont())

        # connect to server
        c.connect()

        # prepare our ports (my machine has 0 <--> 1 with static route)
        # Acquire port 0 for $USER
        c.reset(ports=[0, 1])

        # add both streams to ports
        c.add_streams([s1, s2], ports=[0])

        # clear the stats before injecting
        c.clear_stats()

        # set port 1 as promiscuous mode
        c.set_port_attr(ports=[1], promiscuous=True)

        # choose rate and start traffic for 10 seconds on 14 mpps
        print("Running 14 Mpps on ports 0 for 10 seconds...")
        c.start(ports=[0], mult="14mpps", duration=10)

        # block until done
        c.wait_on_traffic(ports=[0])

        # read the stats after the test
        stats = c.get_stats()

        print(json.dumps(stats[0], indent=4,
                         separators=(',', ': '), sort_keys=True))
        print(json.dumps(stats[1], indent=4,
                         separators=(',', ': '), sort_keys=True))

        lost_a = stats[0]["opackets"] - stats[1]["ipackets"]
        lost_b = stats[1]["opackets"] - stats[0]["ipackets"]

        print("\npackets lost from 0 --> 1:   {0} pkts".format(lost_a))
        print("packets lost from 1 --> 0:   {0} pkts".format(lost_b))

        if (lost_a == 0) and (lost_b == 0):
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


# run the tests
simple_burst()
