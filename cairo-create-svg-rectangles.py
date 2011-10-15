#!/usr/bin/python
import cairo, math

surface = cairo.SVGSurface ("rects.svg",640,480)
ctx = cairo.Context (surface)

ctx.set_source_rgb(1,0,0)
ctx.rectangle(640/4,480/2,10,10)
ctx.fill()
ctx.set_source_rgb(0,1,0)
ctx.rectangle(2*640/4,480/2,10,10)
ctx.fill()
ctx.set_source_rgb(0,0,1)
ctx.rectangle(3*640/4,480/2,10,10)
ctx.fill()
