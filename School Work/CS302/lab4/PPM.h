#ifndef __PPM_H__
#define __PPM_H__

#include <iostream>
#include <fstream>
#include <string>

struct RGB { 
  RGB() { R = 0; G = 0; B = 0; }

  unsigned char R, G, B;
};

class PPM { 
  public:
    PPM() { w = 0; h = 0; max = 255; }
    ~PPM() { delete[] img; }
    
    void read(const string &fname);
    void write(string &fname);
    
    RGB *operator[](int k) { return img[k]; }

    int get_Nrows() { return h; }
    int get_Ncols() { return w; }

  private:
                      // data members
    int w;            // width
    int h;            // height
    int max;          // max intensity

    RGB **img;
};

#endif
