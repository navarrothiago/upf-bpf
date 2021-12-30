import json
import sys

hostname = "http://localhost:1234/"
dir = "test-sessions"

def add_basic_arp_rule():
    return [{"ip": "10.0.0.0","mac": "a0:36:9f:23:ac:2c"}]


def create_arp_table(num, session, n_rules):
    offset = (session - 1) * n_rules + num + 1
    x = int(offset / 256)
    y = offset % 256
    ip = "10.1."+str(x)+"."+str(y)
    return [{"ip":ip,"mac": "a0:36:9f:23:ac:2e"}]

def create_far_rule(num, session, n_rules):
    farid = session * 100 + 1 + 2 * num
    offset = (session - 1) * n_rules + num + 1
    x = int(offset / 256)
    y = offset % 256
    ip = "10.1."+str(x)+"."+str(y)
    far = [{"farId": farid,"forwardingParameters": {"outerHeaderCreation": {"outerHeaderCreationDescription": "OUTER_HEADER_CREATION_GTPU_UDP_IPV4","ipv4Address": ip,"portNumber": 1234},"destinationInterface": "INTERFACE_VALUE_ACCESS"}}]
    farid += 1
    far.extend([{"farId": farid, "forwardingParameters": {"outerHeaderCreation": {"outerHeaderCreationDescription": "OUTER_HEADER_CREATION_UDP_IPV4","ipv4Address": ip,"portNumber": 1234},"destinationInterface": "INTERFACE_VALUE_CORE"}}])
    return far

def create_pdr_rule(num, session, n_rules):
    pdrid = session * 100 + 1 + 2 * num
    farid = pdrid
    teid = pdrid
    precedence = pdrid
    offset = (session - 1) * n_rules + num + 1
    x = int(offset / 256)
    y = offset % 256
    ip = "10.1."+str(x)+"."+str(y)
    pdr = [{ "pdrId":pdrid,"farId":farid, "outerHeaderRemoval": "OUTER_HEADER_REMOVAL_UDP_IPV4", "pdi": {"teid":teid,"sourceInterface": "INTERFACE_VALUE_CORE","ueIPAddress":ip}, "precedence": precedence}]
    pdrid += 1
    farid += 1
    teid += 1
    precedence += 1
    pdr.extend([{"pdrId":pdrid, "farId":farid, "outerHeaderRemoval": "OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4","pdi": {"teid":teid,"sourceInterface": "INTERFACE_VALUE_ACCESS","ueIPAddress":ip}, "precedence": precedence}])
    return pdr

def create_body(session_id, n_rules):
    mydict = {"seid":session_id, "pdrs":[], "fars":[], "arpTable":[]}
    for j in range(n_rules):
        mydict["pdrs"].extend(create_pdr_rule(j, session_id, n_rules))
    # Adding FAR rules
    for j in range(n_rules):
        mydict["fars"].extend(create_far_rule(j, session_id, n_rules))
    mydict["arpTable"].extend(add_basic_arp_rule())
    for j in range(n_rules):
        mydict["arpTable"].extend(create_arp_table(j, session_id, n_rules))
    return mydict

def create_session(session_id, n_rules):
    filename = "session_"+str(session_id)+".json"
    path = dir+"/"+filename
    f = open(path, "w")
    string = create_body(session_id, n_rules)
    json.dump(string, f, indent = 6)
    f.close()

def create_sessions(n_sessions, n_rules):
    sessions_counter = 0
    for i in range(1, int(n_sessions)+1):
        create_session(i, int(n_rules))

def main(argv):
    if len(sys.argv) != 3:
        print("Usage: <N_SESSIONS> <N_RULES_PER_SESSION>")
        sys.exit(2)
    n_sessions = sys.argv[1]
    n_rules = sys.argv[2]
    print("Number of sessions:", n_sessions, "number of rules:", n_rules)
    create_sessions(n_sessions, n_rules)

if __name__ == "__main__":
    main(sys.argv)
