CPPFLAGS=-g -Wall -Wno-reorder
CPPPATH=/Users/tbonfort/dev/agg-2.5/include
LIBS=-L/Users/tbonfort/dev/agg-2.5/src -lagg -lexpat

SRC=svg2ppm.cpp agg_svg_parser.cpp agg_svg_path_renderer.cpp agg_svg_path_tokenizer.cpp

svg2ppm:  $(SRC)
	g++ -o svg2ppm $(CPPFLAGS) -I$(CPPPATH) $(SRC) $(LIBS)
