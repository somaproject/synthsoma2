import pysynthsoma2
from nose.tools import *
import time



def test_tspikewave():
    tsw = pysynthsoma2.TSpikeWave()
    tsw.filtid = 10
    assert_equal(tsw.filtid, 10)

    tsw.wave = range(32)
    
    assert_equal(tsw.wave[3], 3)

def test_tspike():
    ts = pysynthsoma2.TSpike()
    ts.time = 0x12345678
    assert_equal(ts.time, 0x12345678)

    ts.x.filtid = 10;
    assert_equal(ts.x.filtid, 10)
    
