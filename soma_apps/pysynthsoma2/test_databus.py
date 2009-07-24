import pysynthsoma2
from nose.tools import *
import time
import socket, os
import tempfile


def test_timer_and_net_and_tspike():
    dm = pysynthsoma2.EventDeviceMap()
    domaindir = tempfile.mkdtemp(prefix="soma")
    
    ne1 = pysynthsoma2.NetEventServer.createDomain(domaindir)
    dm[4] = ne1

    simplets = pysynthsoma2.SimpleTSpike()
    dm[8] = simplets


    nd1 = pysynthsoma2.NetDataServer.createDomain(domaindir)
    event_bus = pysynthsoma2.EventBus(dm)
    
    ddm = pysynthsoma2.DataDeviceMap()
    ddm[0] = simplets
    data_bus = pysynthsoma2.DataBus(ddm)
    data_bus.setDataSink(nd1)

    # create the destination socket
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)
    os.makedirs(domaindir + "/data/tspike")
    sock.bind(domaindir + "/data/tspike/0")

    runner = pysynthsoma2.Runner(event_bus, data_bus)
    
    runner.run()
    T  = 5
    for i in range(T):
        print "getting" 
        sock.recv(1000)
        s = runner.getStats()
        print s.eventcycles, s.eventcyclerate
    
        
    runner.shutdown()
    s = runner.getStats()
    
