import sys
from somapynet.event import Event
from somapynet import eaddr
from somapynet.neteventio import NetEventIO

import struct
import time

somaIP = "127.0.0.1"

eio = NetEventIO(somaIP)
dev = 40

eio.addRXMask(xrange(256), dev)

eio.start()
        
e = Event()
e.src = eaddr.NETWORK
e.cmd =  0x30
e.data[0] = 1
e.data[1] = 1
e.data[2] = 1
e.data[3] = 0



ea = eaddr.TXDest()
ea[dev] = 1
eio.sendEvent(ea, e)

for i in range(100):
    erx = eio.getEvents(blocking=True)
    for e in erx:
        print e

eio.stop()
        
