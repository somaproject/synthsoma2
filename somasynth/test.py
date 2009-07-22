#!/usr/bin/python

import pygtk
pygtk.require('2.0')
import gobject

import gtk, gtk.glade

class SimpleTest:
    def __init__(self):
        xml = gtk.glade.XML('testgui.glade')
        xml.signal_autoconnect(self)
        self.xml = xml
        
        self.ecyc = self.xml.get_widget("progressbarEventCyclesSec")

        gobject.timeout_add(100, self.timeout)

        self.value = 0.0
        self.inc = 0.1
    def on_button1_clicked(self, button):
        print 'foo!'

    def updateValue(self):
        if self.value > 0.9:
            self.inc = - 0.1
        elif self.value < 0.1:
            self.inc = 0.1

        self.value += self.inc
        self.ecyc.set_fraction(self.value)
        
    def timeout(self):
        print "timeout called"
        self.updateValue()

        return True
test = SimpleTest()
gtk.main()
