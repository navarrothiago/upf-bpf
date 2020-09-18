#!/bin/bash

# Based on - Veth XDP: XDP for Containers (see slides)

NR_CPU=$(nproc)
NAMESPACE=ns0
NAMESPACE2=ns2
PHY_NIC=$1

if [ "$#" -ne 1 ]; then
  echo "Illegal number of parameters"
fi

echo "Remove namespace ns0"
sudo ip netns del ns0

echo "Create a namespace"
sudo ip netns add ns0

# Put veth1 in ns0.
# Number of queues should be equal to the number of CPUs.
echo "Create a veth pair with veth0 and veth1."
sudo ip link add veth0 numrxqueues ${NR_CPU} numtxqueues ${NR_CPU} type veth \
peer name veth1 netns ${NAMESPACE} numrxqueues ${NR_CPU} numtxqueues ${NR_CPU}

echo "Tx vlan off in veth0"
sudo ethtool -K veth0 tx off txvlan off

echo "Tx vlan off in veth1"
sudo ip netns exec ${NAMESPACE} ethtool -K veth1 tx off txvlan off

# FIXME navarrothiago - Not working.
#sudo ethtool -K PHY_NIC rxvlan off

echo "Get veth1 mac address"
MAC_OF_VETH1=$(sudo ip netns exec ns0 cat /sys/class/net/veth1/address)

echo "Create a bridge foward database"
sudo bridge fdb add ${MAC_OF_VETH1} dev ${PHY_NIC} self # Unicast filter

echo "Turn ifaces up"
sudo ip link set veth0 up
sudo ip netns exec ns0 ip link set veth1 up
