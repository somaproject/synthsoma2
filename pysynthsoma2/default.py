#!/usr/bin/python
"""

Default : Create the default instance



"""
import  pysynthsoma2
import socket
import time
import tempfile
import numpy as np

def ip(ipaddr):
    """
    Create default with IP connection

    """

    ne1 = pysynthsoma2.NetEventServer.createINet(ipaddr)    
    nd1 = pysynthsoma2.NetDataServer.createINet(ipaddr)

    return ne1, nd1

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

    return ne1, nd1


def createClusters():
    """
    Create four simple fake spikes based on four noisy cluster profiles.


    """

    proto_spikes_uv = [(300, 100, 150, 100), 
                       (100, 300, 100, 150),
                       (50, 100, 300, 100),
                       (50, 200, 200, 350)]
    noise_uv = 15
    fs = 32000.
    
    def create_cluster(spoints):
        x = np.zeros((32, 4))
        x[8] = np.array(spoints) / 1e6
        x[8] += np.random.normal(0, noise_uv/1e6, 4)
        
        return x
    
    interval_samples = 400 # space between spikes

    samples = []

    ITER = 100
    f0 = 40.
    f1 = 200.
    amp = 1e-3
    t = 0.0 
    for iter in range(ITER):
        for ps in proto_spikes_uv:
            for i in range(interval_samples):
                newsamp = [0 for j in range(10)]
                newsamp[4] = np.sin(2*np.pi * f0 * t) * amp
                newsamp[9] = np.sin(2*np.pi * f1 * t) * amp
                samples.append(newsamp)
                t += 1./(fs)
                
            cluster = create_cluster(ps)

            for samp in cluster:
                newsamp = samp.tolist() + [0.0] + samp.tolist() + [0.0]

                newsamp[4] = np.sin(2*np.pi * f0 * t) * amp
                newsamp[9] = np.sin(2*np.pi * f1 * t) * amp
                samples.append(newsamp)
                t += 1./(fs)
    for s in samples:
        assert(len(s) == 10)
    
    return samples


def createWaves():
    """
    Create a few different sinusoids at 32 kHz for wave tests
    Note that frequently the only source hooked up to the "wavesource" device
    is the continuous output.
    """

    FS = 32000
    times = np.arange(0, FS) / FS

    samples = []
    for t in times:
        f0 = 50
        f1 = 200
        amp = 1e-3
        s = [0., 0., 0., 0., 0., 0., 0., 0., 0., 0.]
        s[4] = np.sin(2*np.pi * f0 * t) * amp
        s[9] = np.sin(2*np.pi * f1 * t) * amp
        samples.append(s)
    print "The length of samples are", len(samples)
    return samples

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
        for j in range(1000):
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

def create_default(ne1, nd1, TSPIKESOURCE_N = 8, WAVESOURCE_N = 1,
                   timestart =0, audio=None):
    """
    Create TSPIKESOURCE_N tspike sources, on data sources
    0 to TSPIKESOURCE_N-1

    Create WAVESOURCE_N wave sources, on data sources
    TSPIKESOURCE_N to TSPIKESOURCE_N+WAVESOURCE_N-1
    
    """
    edm = pysynthsoma2.EventDeviceMap()
    ddm = pysynthsoma2.DataDeviceMap()
    
    #create the timer 
    td = pysynthsoma2.Timer()
    td.setTimeStamp(timestart)
    edm[0] = td

    # create the network event device
        
    edm[4] = ne1

    for src in range(TSPIKESOURCE_N):
        dspboard = pysynthsoma2.DSPBoard()

        # now add data, roughly 20 Hz delta functions of 150uV
        samples = createClusters()
        dspboard.setSampleBuffer(samples)
        edm[8 + src] = dspboard
        ddm[0 + src] = dspboard

    for src in range(TSPIKESOURCE_N, TSPIKESOURCE_N + WAVESOURCE_N):
        dspboard = pysynthsoma2.DSPBoard()
        # add sinusoidal data
        samples = createWaves()
        dspboard.setSampleBuffer(samples)
        edm[8 + src] = dspboard
        ddm[0 + src] = dspboard

    if audio != None:
        a = int(audio)
        aud = pysynthsoma2.AudioDevice()
        edm[a] = aud
            
        
    event_bus = pysynthsoma2.EventBus(edm)
    data_bus = pysynthsoma2.DataBus(ddm)

    data_bus.setDataSink(nd1)

    runner = pysynthsoma2.Runner(event_bus, data_bus)

    return runner
