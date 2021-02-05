#!/bin/bash

# Based on - Veth XDP: XDP for Containers (see slides)

NR_CPU=$(nproc)
NAMESPACE=$1
VETH=$2
PHY_NIC=$3

if [ "$#" -ne 3 ]; then
  echo "Illegal number of parameters"
  exit 1
fi

echo "Remove namespace ${NAMESPACE}"
sudo ip netns del ${NAMESPACE}

echo "Create a namespace"
sudo ip netns add ${NAMESPACE}

# Wait namespace to be create. 
# Workaround: Cannot find device "vethX"
sleep .2

# Put veth2 in ${NAMESPACE}.
# Number of queues should be equal to the number of CPUs.
echo "Create a veth pair with ${VETH} and veth2."
sudo ip link add ${VETH} numrxqueues ${NR_CPU} numtxqueues ${NR_CPU} type veth \
peer name veth2 netns ${NAMESPACE} numrxqueues ${NR_CPU} numtxqueues ${NR_CPU}

echo "Tx vlan off in ${VETH}"
sudo ethtool -K ${VETH} tx off txvlan off

echo "Tx vlan off in veth2"
sudo ip netns exec ${NAMESPACE} ethtool -K veth2 tx off txvlan off

# FIXME navarrothiago - Not working.
#sudo ethtool -K PHY_NIC rxvlan off

echo "Get veth2 mac address"
MAC_OF_VETH1=$(sudo ip netns exec ${NAMESPACE} cat /sys/class/net/veth2/address)

echo "Create a bridge foward database"
sudo bridge fdb add ${MAC_OF_VETH1} dev ${PHY_NIC} self # Unicast filter

echo "Turn ifaces up"
sudo ip link set ${VETH} up
sudo ip netns exec ${NAMESPACE} ip link set veth2 up
