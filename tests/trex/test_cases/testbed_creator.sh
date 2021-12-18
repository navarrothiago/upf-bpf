#create sessions if passed
if test "$#" -eq 2; then
	rm test-sessions/session*
	python3 creator6.py $1 $2
fi

#remove previously attached programs
ip link set eth2 xdp off
ip link set eth3 xdp off

#Setup the network configuration for upf-bpf
curl --noproxy '*' -X POST http://localhost:1234/configure --data "@test-sessions/configure.json"

#Setup the different sessions
for filename in test-sessions/session*; do
	sleep 0.1
	echo $filename
	curl --noproxy '*' -X POST http://localhost:1234/createSession --data "@$filename"
echo ''
done
