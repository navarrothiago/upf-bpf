#!/bin/python

from scapy.all import *
from scapy.contrib.gtp import *

# Create GTP packet
packet = Ether(dst="00:e0:4c:36:1c:d1")/IP(src="172.20.16.99",dst="192.168.15.12")/UDP(dport=2152)/GTP_U_Header(teid=100)/IP(src="10.10.10.10",dst="172.20.16.55",version=4)/UDP()/Raw('x'*20)
packet2 = Ether(src="00:db:df:0c:29:66",dst="00:e0:4c:36:1c:d1")/IP(src="192.168.15.7",dst="192.168.15.12")/UDP(dport=1234)/Raw('x'*20)

# Send GTP packet at Layer2
# see https://0xbharath.github.io/art-of-packet-crafting-with-scapy/scapy/sending_recieving/index.html
r = sendp(packet, loop=1, inter=0.5)
