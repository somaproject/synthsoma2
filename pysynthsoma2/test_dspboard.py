import pysynthsoma2
from nose.tools import *
import time
import tempfile


def test_DSPBoard_buffer():
    """
    Add some data to the buffer, just to check if our type conversion code works. 

    """

    
    dm = pysynthsoma2.EventDeviceMap()

    td = pysynthsoma2.Timer()
    dm[0] = td

    dspb = pysynthsoma2.DSPBoard()
    dm[8] = dspb

    samples = []
    for i in range(1000):
        sampbuf = [1.0 for j in range(10)]
        samples.append(sampbuf)
    dspb.setSampleBuffer(samples)
    
    event_bus = pysynthsoma2.EventBus(dm)
    
    ddm = pysynthsoma2.DataDeviceMap()
    data_bus = pysynthsoma2.DataBus(ddm)

    runner = pysynthsoma2.Runner(event_bus, data_bus)
    
    runner.run()
    T = 2
    time.sleep(T)
    runner.shutdown()

    

def test_DSPBoard_events():
    """
    
    """

    
    dm = pysynthsoma2.EventDeviceMap()

    td = pysynthsoma2.Timer()
    dm[0] = td

    dspb = pysynthsoma2.DSPBoard()
    dm[8] = dspb

    debugdevice = pysynthsoma2.TestDevice()
    debugdevice.ignoreSource(0)
    dm[6] = debugdevice

    
##     samples = []
##     for i in range(1000):
##         sampbuf = [1.0 for j in range(10)]
##         samples.append(sampbuf)
##     dspb.setSampleBuffer(samples)
    
    event_bus = pysynthsoma2.EventBus(dm)
    
    ddm = pysynthsoma2.DataDeviceMap()
    data_bus = pysynthsoma2.DataBus(ddm)

    runner = pysynthsoma2.Runner(event_bus, data_bus)
    
    runner.run()
    T = 2
    time.sleep(T)

    e = debugdevice.getEvents()
    print [(ei.src, ei.cmd) for ei in e]
    
    #assert_equal(e, [])
    runner.shutdown()

    
