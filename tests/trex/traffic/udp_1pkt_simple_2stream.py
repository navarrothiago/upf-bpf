from trex_stl_lib.api import *
from scapy.contrib.gtp import *

class STLS1(object):

    def create_stream (self, src):
        return STLStream(
            packet =
                    STLPktBuilder(
                        pkt = Ether()/IP(src=src,dst="10.1.3.27")/
                                UDP(dport=1234)/Raw('x'*20)
                    ),
            mode = STLTXCont())

    def get_streams (self, direction = 0, **kwargs):
        # create 1 stream
        return [ self.create_stream("10.1.3.1"), self.create_stream("10.1.3.2") ]


# dynamic load - used for trex console or simulator
def register():
    return STLS1()



