//======================================================================
//  svg2ppm.cpp - An example svg to translator using agg.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Sat Oct 15 22:17:37 2011
//----------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string>
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_pixfmt_gray.h"
#include "agg_pixfmt_rgb.h"
#include "agg_color_rgba.h"
#include "agg_svg_parser.h"

using namespace std;

typedef unsigned char u8;

static void die(const char *fmt, ...)
{
    va_list ap;
    va_start(ap,fmt); 
    
    vfprintf(stderr, fmt, ap);
    exit(-1);
}

// An ppm image
class PImg {
  public:
    PImg(int width, int height)
    {
      this->width = width;
      this->height = height;
      buf =  new u8[width * height * 3];
    }
    int width, height;
    u8 *buf;
  
    void save_ppm(const char *filename)
    {
      FILE *fh = fopen(filename, "wb");
      if (!fh) {
        fprintf(stderr, "Failed opening %s for writing!\n", filename);
        exit(-1);
      }
      fprintf(fh,
              "P6\n"
              "# By svg2ppm.cpp\n"
              "%d %d\n"
              "255\n",
              this->width,
              this->height);
      
      fwrite(this->buf, this->width * this->height, 3, fh);
      
      fclose(fh);
    }
};


#define CASE(s) if (!strcmp(s, S_))

int main(int argc, char **argv)
{
    int argp = 1;
    string outfilename;
    bool do_bgr = false;

    while(argp < argc && argv[argp][0] == '-') {
        char *S_ = argv[argp++];

        CASE("-help") {
            printf("svg2ppm - A testing program for agg-svg \n\n"
                   "Syntax:\n"
                   "    agg-svg [] svg-file\n"
                   "\n"
                   "Options:\n"
                   "    -o outfilename   Set outfilename\n"
                   "    -bgr             Set bgr output mode\n"
                   
                   );
            exit(0);
        }
        CASE("-o") {
            outfilename = argv[argp++];
            continue;
        }
        CASE("-bgr") {
            do_bgr = true;
            continue;
        }
        die("Unknown option %s!\n", S_);
    }

    if (argp >= argc)
        die("Need svg filename!\n");

    string svgfilename = argv[argp++];
    
    // Parse and draw svg file
    agg::svg::path_renderer path;
    agg::svg::parser p(path);
    p.set_swap_red_blue(do_bgr);

    try {
      p.parse(svgfilename.c_str());
    }
    catch (agg::svg::exception e) {
      die(e.msg());
    }
    double mx,my,Mx,My;
    path.bounding_rect(&mx,&my,&Mx,&My);
    PImg img(Mx+1,My+1);
    if (outfilename.size() == 0)
      outfilename = svgfilename + ".ppm";
    agg::trans_affine mtx;

    typedef agg::pixfmt_rgb24 pixfmt;
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
    agg::rendering_buffer rbuf(img.buf,
                               img.width,
                               img.height,
                               img.width*3);
    agg::pixfmt_rgb24 pixf(rbuf);
    renderer_base rbase(pixf);
    agg::rasterizer_scanline_aa<> pf;
    agg::scanline_p8 sl;
    renderer_solid ren(rbase);
    
    // Draw a white background in the buffer
    pf.move_to_d(0,0);
    pf.line_to_d(img.width,0);
    pf.line_to_d(img.width,img.height);
    pf.line_to_d(0,img.height);
    agg::render_scanlines_aa_solid(pf, sl, rbase,
                                   agg::rgba8(0xff,0xff,0xff,0xff));
    
    
    path.render(pf, sl, ren, mtx, 1.0);

    img.save_ppm(outfilename.c_str());
    exit(0);
    return(0);
}


