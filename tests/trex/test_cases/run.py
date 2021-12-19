#!/bin/python3

# import stl_path
from tokenize import String
from unittest import result
from zipfile import Path
from trex_stl_lib.api import *
import numpy as np
from scapy.contrib.gtp import *

import time
import json
import argparse
import subprocess
from collections import defaultdict

item = defaultdict(dict)


def create_udp_pkt_flow(size, ip_min, ip_max, nflows, field):
    print("{} flow will be generated...".format(nflows))
    base_pkt = Ether()/IP(src="16.0.0.1", dst="10.1.3.27")/UDP(dport=1234)
    pad = max(0, size - len(base_pkt)) * 'x'
    return STLPktBuilder(pkt=base_pkt/pad, vm=create_vm(ip_min, ip_max, nflows, field))

# TODO navarrothiago - update packet


def create_gtp_pkt_flow(size, ip_min, ip_max, nflows, field):
    print("{} flow will be generated...".format(nflows))
    base_pkt = Ether()/IP(src="10.1.3.27", dst="192.168.15.12")/UDP(dport=2152) / \
        GTP_U_Header(teid=100) / \
        IP(src="10.10.10.10", dst="10.1.3.27", version=4)/UDP(dport=1234)
    pad = max(0, size - len(base_pkt)) * 'x'
    return STLPktBuilder(pkt=base_pkt/pad, vm=create_vm(ip_min, ip_max, nflows, field))


def create_vm(ip_min, ip_max, nflows, field):
    vm = STLVM()

    # create a tuple var
    vm.tuple_var(name="tuple", ip_min=ip_min, ip_max=ip_max,
                 port_min=1234, port_max=1234, limit_flows=nflows)

    # write fields
    vm.write(fv_name="tuple.ip", pkt_offset="IP.{}".format(field))
    vm.fix_chksum()

    # vm.write(fv_name="tuple.port", pkt_offset="UDP.sport")
    return vm


def create_pkt(size):
    # simple packet creation
    print("Only one flow will be generated...")
    pkt = Ether()/IP(src="10.1.2.27", dst="10.1.3.27")/UDP(dport=1234)
    pad = max(0, size - len(pkt)) * 'x'
    return STLPktBuilder(pkt=pkt/pad)


def simple_burst(streams, m, duration):

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

        while(1):
            # prepare our ports (my machine has 0 <--> 1 with static route)
            # Acquire port 0 for $USER
            c.reset(ports=[0, 1])

            # add both streams to ports
            c.add_streams(streams, ports=[0])

            # clear the stats before injecting
            c.clear_stats()

            # set port 1 as promiscuous mode
            c.set_port_attr(ports=[1], promiscuous=True)

            # choose rate and start traffic for 10 seconds on 5 mpps
            print("Running " + m + " on ports 0 for {} seconds...".format(duration))
            c.start(ports=[0], mult=m, duration=duration)
            run_mpstat(duration/2)

            # block until done
            c.wait_on_traffic(ports=[0])

            # read the stats after the test
            stats = c.get_stats()

            # print(json.dumps(stats[0], indent=4, separators=(',', ': '), sort_keys=True))
            # print(json.dumps(stats[1], indent=4, separators=(',', ': '), sort_keys=True))

            # print("\n")
            # print("Packets sent       0 --> 1:   {0} pkts".format(stats[0]["opackets"]))
            # print("Rx Mpps            0 --> 1:   {0} Mpps".format(float(stats[1]["rx_pps"])/1000000))

            item["throughput"] = float(stats[1]["rx_pps"])/1000000
            item["loss"] = float(stats[0]["opackets"] - stats[1]["ipackets"])/stats[0]["opackets"]
            print("")
            print("Throughput: {} Mpps".format(item["throughput"]))

            if (item["throughput"] > 2):
                break

            print("\nTest has failed :-(\n")
            print("Error - throughput expected > 2mpps, but got {}".format(item["throughput"]))
            print("Trying again... ")
    except STLError as e:
        print(e)

    finally:
        c.disconnect()

    print("\nTest has passed :-)\n")


def run_mpstat(duration):
    global current_test
    cmd = 'ssh india mpstat -P ALL {} 1 -o JSON'.format(int(duration))
    print("Running mpstat... {}".format(cmd))
    output = os.popen(cmd).read()
    # print(json.loads(output))
    item["mpstat"] = json.loads(output)

# TODO navarrothiago - create a class ethtool.


def run_ethtool_set_rx_queue(number_rx_queue, password):
    print("Setting NIC rx queue size...")
    ifaces = ["enp3s0f0", "enp3s0f1"]
    dicIface = {}
    for iface in ifaces:
        dicIface[iface] = int(number_rx_queue)
        cmd = 'echo {} | ssh india sudo -S ethtool -L {} combined {}'.format(
            password, iface, int(number_rx_queue))
        print("Running ethtool... {}".format(cmd))
        os.popen(cmd)
        cmd = 'echo {} | ssh india sudo -S ethtool -l {} '.format(
            password, iface)
        print("Running ethtool... {}".format(cmd))
        print(os.popen(cmd).read())
        # TODO navarrothiago - Check if it is worth to store this value
        # d[current_test]["rx_queue"] = number_rx_queue
    item["ethtool"] = dicIface
    print("Setting NIC rx queue size...DONE")
    print()


def setup_test_case(name):
    global current_test
    current_test = name
    print("Setup TestCase: {}".format(name))

# TODO navarrothiago - create a file to parse the report.


def get_throughputs(json_output):
    dict_output = json.load(json_output)
    array = []
    for item in dict_output["items"]:
        array.append(item["throughput"])
    return np.array(array)


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
                    default=10,
                    help="The duration of the transmission in second")
parser.add_argument('-f',
                    '--flows',
                    default='udp',
                    help="The flows type (i.e. udp or gtp)")
parser.add_argument("-a",
                    "--auto",
                    help="Ignore all arguments and run in mode automatic",
                    action="store_true")
parser.add_argument('-p',
                    '--password',
                    default="",
                    help="Password of the DUT host")
args = parser.parse_args()

json_output = {
    "items": []
}


current_test = ""
flow_list = [1000]
# flow_list = np.geomspace(1, 1000, num=4, dtype=int)
# n_rx_queues = np.arange(1, 13, 1)
n_rx_queues = np.arange(1, 7, 1)
# n_rx_queues = np.geomspace(1, 10, num=1, dtype=int)

timestr = time.strftime("%Y%m%d-%H%M%S")

test_dict = {
    "udp": {
        "createFlows": create_udp_pkt_flow,
        "testCaseName": "DownlinkMaxThoughtput",
        # source IP will be change in trex machine.
        "ipTarget": "src"
    },
    "gtp": {
        "createFlows": create_gtp_pkt_flow,
        "testCaseName": "UplinkMaxThoughtput",
        # destination IP will be change in trex machine.
        "ipTarget": "dst"
    }
}

# TODO navarrothiago - the number of flows does not correspond to the what it supposed to be.

test_case_name = test_dict[args.flows]["testCaseName"]



for flow in flow_list:
    for rx_size in n_rx_queues:
        item = defaultdict(dict)
        run_ethtool_set_rx_queue(rx_size, args.password)
        for i in {0, 1}:
            tx_data_rate=0
            if i == 0:
                print("Executing with max throughput in order to find the saturation.")
                print("The packet loss and CPU load will increase!!")
                tx_data_rate=args.multiplier
            else:
                print("Executing with the target throughput in order to avoiding packet loss")
                print("The packet loss and CPU load will be fine now!!")
                tx_data_rate=str(item["throughput"]) + "mpps"
            test_case = "{}-{}-{}flow-{}rx".format(timestr, test_case_name, flow, rx_size)
            # test_case = test_case_name
            item["testCase"] = test_case
            setup_test_case("{}".format(test_case))
            s1 = STLStream(packet=test_dict[args.flows]["createFlows"](args.size, "16.0.0.1",
                                                        "16.0.0.254", int(flow), test_dict[args.flows]["ipTarget"]), mode=STLTXCont())
            simple_burst([s1], tx_data_rate, args.duration)

            if i != 0:
                json_output["items"].append(item)

reports_path = "tests/reports"
filename = "{}-{}.json".format(timestr, test_case)
file_to_open = os.path.join(reports_path, filename)
with open(file_to_open, "w") as dump_file:
    json.dump(json_output, dump_file, indent=2,
              separators=(',', ': '), sort_keys=True)

# Update the last report.
tmp_link = os.path.join(reports_path, test_case_name + ".tmp")
target_link = os.path.join(reports_path, test_case_name + ".json")
os.symlink(filename, tmp_link)
os.rename(tmp_link, target_link)
