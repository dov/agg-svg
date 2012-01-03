# agg-svg --- Extension of the agg svg parser

* Author: Dov Grobgeld <dov.grobgeld@gmail.com>
* Created: Saturday 2011-10-15 
* Keywords: agg, svg
* URL: <http://github.com/dov/agg-svg>

# Commentary

Agg comes with an example svg parser. This repository extends the syntax 
of the svg parser with the following features:

* Support for the path element arch.
* Support for the color syntax rgb() and rgba() - as output e.g. in the svg writer of cairo.

## Test programs

* The test program `svg2ppm.cpp` shows an example of how to use the agg svg parser.
* The python program `cairo-create-svg-rectangles.py` creates an example svg file.

# Dependencies

Agg

# Installation

This is a source library. Just drop it in your source tree.
