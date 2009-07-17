import pysynthsoma2
from nose.tools import *
import time
import socket, os
import tempfile


def test():

    domaindir = tempfile.mkdtemp(prefix="soma")
    print "The domain dir = ", domaindir
    

    edm = pysynthsoma2.EventDeviceMap()
    ddm = pysynthsoma2.DataDeviceMap()
    
    #create the timer 
    td = pysynthsoma2.Timer()
    edm[0] = td

    # create the network event device
    ne1 = pysynthsoma2.NetEventServer.createDomain(domaindir)
    edm[4] = ne1

    # create the tspikes device
    
    simplets = pysynthsoma2.SimpleTSpike(1000)
    edm[8] = simplets
    ddm[0] = simplets

    event_bus = pysynthsoma2.EventBus(edm)
    data_bus = pysynthsoma2.DataBus(ddm)

    nd1 = pysynthsoma2.NetDataServer.createDomain(domaindir)
    
    data_bus.setDataSink(nd1)

    runner = pysynthsoma2.Runner(event_bus, data_bus)

    # create the missing sockets FIXME:
    needed = ['dataretx', 'eventretx', 'eventtx']
    socks = []
    for n in needed:
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)
        sock.bind(domaindir + "/" + n)
        socks.append(sock)

    
    runner.run()
    while(True) :
        time.sleep(1)
        s = runner.getStats()
        print s.eventcycles, s.eventcyclerate
    
        
    runner.shutdown()
    s = runner.getStats()
    

if __name__ == "__main__":
    test()
