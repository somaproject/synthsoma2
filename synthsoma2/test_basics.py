import pysynthsoma2
from nose.tools import *
import time

def test_runnerstats():

    r = pysynthsoma2.runnerstats()
    assert_equal(r.eventcycles, 0)


def test_event():
    e = pysynthsoma2.Event()
    etx = pysynthsoma2.EventTX()

def test_testdevice():
    td = pysynthsoma2.TestDevice()
    s = td.getEvents()

    assert_equal(s, [])

def test_databus():
    dm = pysynthsoma2.DataDeviceMap()

    db = pysynthsoma2.DataBus(dm)

def test_eventbus():
    dm = pysynthsoma2.EventDeviceMap()

    td = pysynthsoma2.TestDevice()
    dm[2] = td

    eb = pysynthsoma2.EventBus(dm)
    

def test_startandshutdown():
    edm = pysynthsoma2.EventDeviceMap()

    event_bus = pysynthsoma2.EventBus(edm)

    ddm = pysynthsoma2.DataDeviceMap()
    data_bus = pysynthsoma2.DataBus(ddm)

    runner = pysynthsoma2.Runner(event_bus, data_bus)
    
    runner.run()
    runner.shutdown()


def test_synthsoma():
    edm = pysynthsoma2.EventDeviceMap()

    td = pysynthsoma2.TestDevice()
    edm[2] = td

    event_bus = pysynthsoma2.EventBus(edm)

    ddm = pysynthsoma2.DataDeviceMap()
    data_bus = pysynthsoma2.DataBus(ddm)

    runner = pysynthsoma2.Runner(event_bus, data_bus)
    
    runner.run()
    T = 2
    time.sleep(T)
    runner.shutdown()
    s = runner.getStats()
    assert_true(s.eventcycles > 50000 * 0.9 * T)

def test_timer():
    dm = pysynthsoma2.EventDeviceMap()

    td = pysynthsoma2.Timer()
    dm[0] = td

    event_bus = pysynthsoma2.EventBus(dm)
    
    ddm = pysynthsoma2.DataDeviceMap()
    data_bus = pysynthsoma2.DataBus(ddm)

    runner = pysynthsoma2.Runner(event_bus, data_bus)
    
    runner.run()
    T = 2
    time.sleep(T)
    runner.shutdown()
    s = runner.getStats()
    assert_true(s.eventcycles > 50000 * 0.9 * T)
    assert_true(s.eventsenttotal, T * 50000 * 0.9)
    
def test_timer_and_net():
    dm = pysynthsoma2.EventDeviceMap()

    td = pysynthsoma2.Timer()
    dm[0] = td
    
    ne1 = pysynthsoma2.NetEventServer.createDomain("/tmp/testtwo")
    dm[4] = ne1

    event_bus = pysynthsoma2.EventBus(dm)
    
    ddm = pysynthsoma2.DataDeviceMap()
    data_bus = pysynthsoma2.DataBus(ddm)

    runner = pysynthsoma2.Runner(event_bus, data_bus)
    
    runner.run()
    T  = 5
    for i in range(T):
        time.sleep(1)
        s = runner.getStats()
        print s.eventcycles, s.eventcyclerate
    
        
    runner.shutdown()
    s = runner.getStats()
    assert_true(s.eventcycles > 50000 * 0.9 * T)
    assert_true(s.eventsenttotal, T * 50000 * 0.9)
    

