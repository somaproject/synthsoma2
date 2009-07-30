#!/usr/bin/python
"""

Default : Create the default instance



"""
import  pysynthsoma2
import socket
import time
import tempfile


def ip(ipaddr, TSPIKESOURCEN=8):
    """
    Create default with IP connection

    """

    ne1 = pysynthsoma2.NetEventServer.createINet(ipaddr)    
    nd1 = pysynthsoma2.NetDataServer.createINet(ipaddr)

    return create_default(ne1, nd1, TSPIKESOURCEN)

def domain(rootdir = None):
    if rootdir == None:
        rootdir = tempfile.mkdtemp(prefix="soma")
    

    ne1 = pysynthsoma2.NetEventServer.createDomain(rootdir)
    nd1 = pysynthsoma2.NetDataServer.createDomain(rootdir)        

    # create the missing sockets FIXME:
    needed = ['dataretx', 'eventretx']
    socks = []
    for n in needed:
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)
        sock.bind(rootdir + "/" + n)
        socks.append(sock)

    return create_default(ne1, nd1)


def create_default_simple_tspike(ne1, nd1, TSPIKESOURCE_N = 8):

    edm = pysynthsoma2.EventDeviceMap()
    ddm = pysynthsoma2.DataDeviceMap()
    
    #create the timer 
    td = pysynthsoma2.Timer()
    edm[0] = td

    # create the network event device
        
    edm[4] = ne1

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

    data_bus.setDataSink(nd1)

    runner = pysynthsoma2.Runner(event_bus, data_bus)

    return runner

def create_default(ne1, nd1, TSPIKESOURCE_N = 8):

    edm = pysynthsoma2.EventDeviceMap()
    ddm = pysynthsoma2.DataDeviceMap()
    
    #create the timer 
    td = pysynthsoma2.Timer()
    edm[0] = td

    # create the network event device
        
    edm[4] = ne1

    for src in range(TSPIKESOURCE_N):
        simplets = pysynthsoma2.DSPBoard()

##         # create the tspikes device
##         for j in range(10):
##             ts = pysynthsoma2.TSpike()
##             ts.src = 0;
##             ts.time = 50 * (j+1);
##             ts.x.wave = [1000 * j * i for i in range(32)]
##             ts.y.wave = [1000 * j * i for i in range(32)]
##             ts.a.wave = [1000 * j * i for i in range(32)]
##             ts.b.wave = [1000 * j * i for i in range(32)]


##             simplets.addTSpike(ts)
        edm[8 + src] = simplets
        ddm[0 + src] = simplets

    event_bus = pysynthsoma2.EventBus(edm)
    data_bus = pysynthsoma2.DataBus(ddm)

    data_bus.setDataSink(nd1)

    runner = pysynthsoma2.Runner(event_bus, data_bus)

    return runner
