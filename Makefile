#modify this to match the location of agg source dir
AGG_HOME=$(HOME)/dev/agg-2.5

CPPFLAGS=-g -Wall -Wno-reorder
CPPPATH=$(AGG_HOME)/include
LIBS=-L$(AGG_HOME)/src -lagg -lexpat

SRC=svg2ppm.cpp agg_svg_parser.cpp agg_svg_path_renderer.cpp agg_svg_path_tokenizer.cpp

svg2ppm:  $(SRC) agg_svg_path_renderer.h
	g++ -g -o svg2ppm $(CPPFLAGS) -I$(CPPPATH) $(SRC) $(LIBS)

clean:
	rm -f svg2ppm

