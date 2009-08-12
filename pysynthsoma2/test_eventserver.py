import pysynthsoma2
from nose.tools import *
import time
import tempfile
import default

## def test_eventserver_domain():

##     rootdir = 
##     ne1 = pysynthsoma2.NetEventServer.createDomain(rootdir)

##     testdev = pysynthsoma2.TestDevice()
    
##     edm = pysynthsoma2.EventDeviceMap()
##     ddm = pysynthsoma2.DataDeviceMap()
    

        
##     edm[4] = ne1
##     edm[8] = testdev

##     event_bus = pysynthsoma2.EventBus(edm)
##     data_bus = pysynthsoma2.DataBus(ddm)

##     runner = pysynthsoma2.Runner(event_bus, data_bus)

##     # now we try a loopback event.

##     runner.run()
##     while(True) :
##         time.sleep(1)
##         s = runner.getStats()

        
##     runner.shutdown()

