import pysynthsoma2
from nose.tools import *
import time
import socket, os
import tempfile


def test():

    useip =  False
    if useip:
        ip = "127.0.0.1"
        
    domaindir = tempfile.mkdtemp(prefix="soma")
    print "The domain dir = ", domaindir
    

    edm = pysynthsoma2.EventDeviceMap()
    ddm = pysynthsoma2.DataDeviceMap()
    
    #create the timer 
    td = pysynthsoma2.Timer()
    edm[0] = td

    # create the network event device
    if useip:
        ne1 = pysynthsoma2.NetEventServer.createINet(ip)
    else:
        ne1 = pysynthsoma2.NetEventServer.createDomain(domaindir)
        
    edm[4] = ne1

    TSPIKESOURCE_N = 16
    for src in range(TSPIKESOURCE_N):
        simplets = pysynthsoma2.SimpleTSpike()

        # create the tspikes device
        for j in range(10):
            ts = pysynthsoma2.TSpike()
            ts.src = 0;
            ts.time = 50 * (j+1);
            ts.x.wave = [1000 * j * i for i in range(32)]
            ts.y.wave = [1000 * j * i for i in range(32)]
            ts.a.wave = [1000 * j * i for i in range(32)]
            ts.b.wave = [1000 * j * i for i in range(32)]


            simplets.addTSpike(ts)
        edm[8 + src] = simplets
        ddm[0 + src] = simplets

    event_bus = pysynthsoma2.EventBus(edm)
    data_bus = pysynthsoma2.DataBus(ddm)

    if useip:
        nd1 = pysynthsoma2.NetDataServer.createINet(ip)
    else:
        nd1 = pysynthsoma2.NetDataServer.createDomain(domaindir)        
    data_bus.setDataSink(nd1)

    runner = pysynthsoma2.Runner(event_bus, data_bus)

    # create the missing sockets FIXME:
    needed = ['dataretx', 'eventretx']
    socks = []
    for n in needed:
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)
        sock.bind(domaindir + "/" + n)
        socks.append(sock)

    
    runner.run()
    while(True) :
        time.sleep(1)
        s = runner.getStats()
        print s.eventcycles, s.eventcyclerate, s.datapackets, s.datapacketrate
    
        
    runner.shutdown()
    s = runner.getStats()
    

if __name__ == "__main__":
    test()
