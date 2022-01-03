#create sessions if passed
# ./testbed_creator.sh <gtp/udp> <#pfcp-session> <#rules-per-session>

if test "$#" -lt 3; then
  echo "Usage:"
  echo "./$0 <gtp/udp> <#pfcp-session> <#rules-per-session>"
  exit 1
fi
if test "$#" -eq 2; then
	rm test-sessions/session*
	python3 creator6.py $1 $2
fi

#remove previously attached programs
ip link set eth2 xdp off
ip link set eth3 xdp off

#Setup the network configuration for upf-bpf
curl --noproxy '*' -X POST http://localhost:1234/configure --data "@test-sessions/configure-$3.json"

#Setup the different sessions
for filename in test-sessions/session*; do
	sleep 0.1
	echo $filename
	curl --noproxy '*' -X POST http://localhost:1234/createSession --data "@$filename"
echo ''
done
