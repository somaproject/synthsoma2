#!/usr/bin/env python
"""
show how to add a matplotlib FigureCanvasGTK or FigureCanvasGTKAgg widget to a
gtk.Window
"""

import gtk
import gobject
from matplotlib.figure import Figure
from numpy import arange, sin, pi
import numpy as np

# uncomment to select /GTK/GTKAgg/GTKCairo
#from matplotlib.backends.backend_gtk import FigureCanvasGTK as FigureCanvas
from matplotlib.backends.backend_gtkagg import FigureCanvasGTKAgg as FigureCanvas
#from matplotlib.backends.backend_gtkcairo import FigureCanvasGTKCairo as FigureCanvas

class TestGUI():
    def __init__(self):

        win = gtk.Window()
        win.connect("destroy", lambda x: gtk.main_quit())
        win.set_default_size(400,300)
        win.set_title("Embedding in GTK")


        vbox = gtk.VBox()
        win.add(vbox)
        self.axes = []
        self.lines = []
        self.canvases = []
        self.t = arange(0.0,3.0,0.01)
        fign = 3
        for f in range(fign):
            f = Figure(figsize=(5,4), dpi=100)
            f.subplots_adjust(bottom=0., top=1., left=0., right=1.)
            a = f.add_subplot(111)
            
            self.axes.append(a)
            #a.set_axe

            s = sin(2*pi*self.t)
            l, = a.plot(self.t,s)
            self.lines.append(l)
            a.grid(True) 
            #a.set_yticks([])
            #a.set_xticks([])
            #a.set_axis_off()

            canvas = FigureCanvas(f)  # a gtk.DrawingArea
            self.canvases.append(canvas)
            vbox.pack_start(canvas)


        win.show_all()
        self.phase  = 0.0
        gobject.timeout_add(500, self.timeout)
        

    def timeout(self):
        "do something"
        self.phase += 0.1
        for l in self.lines:
            l.set_ydata(np.sin(2*pi*self.t + self.phase))
        for c in self.canvases:
            c.draw()
        return True

g = TestGUI()       
gtk.main()
