# A SConstruct for my Fedora 21 box
env = Environment(CPPFLAGS=['-g','-Wall','-Wno-reorder'],
                  CPPPATH=['/usr/include/agg2'],
                  LIBS=['agg','expat'])

env.Program('svg2ppm',
            ['svg2ppm.cpp',
             'agg_svg_parser.cpp',
             'agg_svg_path_renderer.cpp',
             'agg_svg_path_tokenizer.cpp',
             ])


