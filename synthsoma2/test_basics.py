import pysynthsoma2
from nose.tools import *

def test_runnerstats():

    r = pysynthsoma2.runnerstats()
    assert_equal(r.eventcycles, 0)


    
