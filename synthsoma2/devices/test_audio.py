import sys
from somapynet.event import Event
from somapynet import eaddr
from somapynet.neteventio import NetEventIO
from matplotlib import pyplot

import struct
import time

somaIP = "127.0.0.1"

eio = NetEventIO(somaIP)
dev = int(sys.argv[1])
print "Listening to dev=", dev

eio.addRXMask(xrange(256), dev)

eio.start()
        
e = Event()
e.src = eaddr.NETWORK
e.cmd =  0x30
e.data[0] = 1 # this is a state set
e.data[1] = 1 # enabled 
e.data[2] = 2 # chansel 
e.data[3] = 0

ea = eaddr.TXDest()
ea[dev] = 1
eio.sendEvent(ea, e)


## ## # now we send the "set gain" event and wait for response
## e = Event()
## e.src = eaddr.NETWORK
## e.cmd =  0x41
## e.data[0] = 2
## e.data[1] = 0x1F # all four channels
## e.data[2] = 00  # gain = 100
## e.data[3] = 100

## ea = eaddr.TXDest()
## ea[dev] = 1

## eio.sendEvent(ea, e)

data = []

for i in range(100):
    erx = eio.getEvents(blocking=True)
    for e in erx:
        if e.cmd == 0x18:
            for j in range(1, 5):
                s = e.data[j]
                if s >= 2**15:
                    s -= 2**16
                data.append(s)
eio.stop()

pyplot.plot(data)
pyplot.show()
        
