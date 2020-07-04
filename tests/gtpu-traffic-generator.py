#!/bin/python

from scapy.all import *
from scapy.contrib.gtp import *

# Create GTP packet
packet = Ether(dst="00:e0:4c:53:44:58")/IP(src="172.20.16.99",dst="192.168.15.3")/UDP(dport=2152)/GTP_U_Header(teid=1234)/IP(src="10.10.10.10",dst="172.20.16.55",version=4)/UDP()/Raw('x'*20)


# Send GTP packet at Layer2 
# see https://0xbharath.github.io/art-of-packet-crafting-with-scapy/scapy/sending_recieving/index.html
r = sendp(packet, loop=1, inter=1)