import pysynthsoma2
from nose.tools import *
import time
import tempfile
import default

pysynthsoma2.setlogging()

e, d = default.ip("127.0.0.1")
runner = default.create_default(e, d, 1)

runner.run()
while(True) :
    time.sleep(1)
    s = runner.getStats()
    #print s.eventcycles, s.eventcyclerate, s.datapackets, s.datapacketrate
    
runner.shutdown()
s = runner.getStats()
