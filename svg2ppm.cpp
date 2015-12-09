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

class Img {
  public:
    Img(int width, int height)
      : width(width),
        height(height) {}

    int width, height;
    u8 *buf;
};

// An ppm image
class PImg : public Img{
  public:
    PImg(int width, int height)
      : Img(width,height)
    {
      buf =  new u8[width * height * 3];
    }
  
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

// An ppm image
class GImg : public Img{
  public:
    GImg(int width, int height)
      : Img(width, height)
    {
      buf =  new u8[width * height * 3];
    }
  
    void save_pgm(const char *filename)
    {
      FILE *fh = fopen(filename, "wb");
      if (!fh) {
        fprintf(stderr, "Failed opening %s for writing!\n", filename);
        exit(-1);
      }
      fprintf(fh,
              "P5\n"
              "# By svg2ppm.cpp\n"
              "%d %d\n"
              "255\n",
              this->width,
              this->height);
      
      fwrite(this->buf, this->width * this->height, 1, fh);
      
      fclose(fh);
    }
};


#define CASE(s) if (!strcmp(s, S_))

int main(int argc, char **argv)
{
    int argp = 1;
    string outfilename;
    bool do_bgr = false;
    double scale = 1.0;
    bool do_gray = false;

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
                   "    -s scale         Scale the output\n"
                   
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
        CASE("-s") {
            scale = atof(argv[argp++]);
            continue;
        }
        CASE("-gray") {
          do_gray = true;
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
    printf("width height in mm = %f %f\n", p.width_in_mm(), p.height_in_mm());
    const double *view_box = p.view_box();
    printf("view_box = %f %f %f %f\n", view_box[0],view_box[1],view_box[2],view_box[3]);
    double mx,my,Mx,My;
    path.bounding_rect(&mx,&my,&Mx,&My);
    Mx*= scale;
    My*= scale;

    if (do_gray)
    {
      GImg img(Mx+1,My+1);
      if (outfilename.size() == 0)
        outfilename = svgfilename + ".pgm";
      agg::trans_affine mtx;
      mtx.scale(scale);
      typedef agg::pixfmt_gray8 pixfmt;
      typedef agg::renderer_base<pixfmt> renderer_base;
      agg::rendering_buffer rbuf(img.buf,
                                 img.width,
                                 img.height,
                                 img.width);
      agg::pixfmt_gray8 pixf(rbuf);
      renderer_base rbase(pixf);
      agg::rasterizer_scanline_aa<> pf;
      agg::scanline_p8 sl;
      agg::rect_i cb(0,0,1000,1000);
      
      // Draw a white background in the buffer
      pf.move_to_d(0,0);
      pf.line_to_d(img.width,0);
      pf.line_to_d(img.width,img.height);
      pf.line_to_d(0,img.height);
      agg::render_scanlines_aa_solid(pf, sl, rbase, agg::gray8(0xff));
      
      path.render(pf, sl, rbase, mtx, cb);
  
      img.save_pgm(outfilename.c_str());
    }
    else
    {
      PImg img(Mx+1,My+1);
      if (outfilename.size() == 0)
        outfilename = svgfilename + ".ppm";
      agg::trans_affine mtx;
      mtx.scale(scale);
      typedef agg::pixfmt_rgb24 pixfmt;
      typedef agg::renderer_base<pixfmt> renderer_base;
      agg::rendering_buffer rbuf(img.buf,
                                 img.width,
                                 img.height,
                                 img.width*3);
      pixfmt pixf(rbuf);
      renderer_base rbase(pixf);
      agg::rasterizer_scanline_aa<> pf;
      agg::scanline_p8 sl;
      agg::rect_i cb(0,0,1000,1000);
      
      // Draw a white background in the buffer
      pf.move_to_d(0,0);
      pf.line_to_d(img.width,0);
      pf.line_to_d(img.width,img.height);
      pf.line_to_d(0,img.height);
      agg::render_scanlines_aa_solid(pf, sl, rbase, agg::rgba8(0xff,0xff,0xff,0xff));
      
      path.render(pf, sl, rbase, mtx, cb);
  
      img.save_ppm(outfilename.c_str());
    }
    exit(0);
    return(0);
}


