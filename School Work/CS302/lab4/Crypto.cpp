#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
using namespace std;

#include "PPM.h"
#include "Rnumgen.h"

const char ETX = 0x3;

struct pixel { 
  int x;
  int y; 
}; 

void set_pixel_list(PPM &img, vector <pixel> &A, int s) {
  unsigned char r6, r5, r4, r3, g6, g5, 
                g4, g3, b6, b5, b4, b3;
  
  int value;
  vector <int> histo(4096, 0);
  
  // determine image size
  int h = img.get_Nrows(); // height
  int w = img.get_Ncols(); // width
  
  pixel p;

  for (int i = 0; i < h; i++) { 
    for (int j = 0; j < w; j++) {
      p.x = j;
      p.y = i; 
      A.push_back(p); 

      // histogram for crypto2
      r6 = (img[i][j].R >> 6) & 0x1;
      r5 = (img[i][j].R >> 5) & 0x1;
      r4 = (img[i][j].R >> 4) & 0x1;
      r3 = (img[i][j].R >> 3) & 0x1;
      g6 = (img[i][j].G >> 6) & 0x1;
      g5 = (img[i][j].G >> 5) & 0x1;
      g4 = (img[i][j].G >> 4) & 0x1;
      g3 = (img[i][j].G >> 3) & 0x1;
      b6 = (img[i][j].B >> 6) & 0x1;
      b5 = (img[i][j].B >> 5) & 0x1;
      b4 = (img[i][j].B >> 4) & 0x1;
      b3 = (img[i][j].B >> 3) & 0x1;
      value = (r6 << 11) | (r5 << 10) | (r4 << 9) | (r3 << 8) | 
              (g6 << 7) | (g5 << 6) | (g4 << 5) | (g3 << 4) | 
              (b6 << 3) | (b5 << 2) | (b4 << 1) | b3;
      histo[value]++;
    }
  }
  // randnumgen based on histogram
  rnumgen R(s, histo);
  R.pdf(s, histo);
  int r_1 = R.rand();  int r_2 = R.rand();
  int r_24 = (r_1 << 12) | r_2;
 
  // "randperm idea"
  for (int i = (int)A.size()-1; i > 0; --i) { 
    swap(A[i], A[r_24 % (i+1)]);
  }
  
  return;
}

void encode(PPM &img, vector<pixel> &A) {
  char a;
  unsigned char b6, b5, b4, b3, b2, b1, b0;
  int x;
  int y;
  int alt = 0; // R, C, or B? 
  int loc = 0; // which pixel?
  pixel p;
  vector <char> C;

// read from stdin
  while (cin.get(a)) {

    // extract bits from character
    C.clear();
    C.resize(7);
    b0 = a & 0x1;        C[0] = b0;
    b1 = (a >> 1) & 0x1; C[1] = b1;
    b2 = (a >> 2) & 0x1; C[2] = b2;
    b3 = (a >> 3) & 0x1; C[3] = b3;
    b4 = (a >> 4) & 0x1; C[4] = b4;
    b5 = (a >> 5) & 0x1; C[5] = b5;
    b6 = (a >> 6) & 0x1; C[6] = b6;

    for (int i = 0; i <= 6; i++) {
      if ((loc + i) >= A.size()) { return; }
      p = A[loc+i];
      if ((alt % 3) == 0) img[p.y][p.x].R = C[i];
      if ((alt % 3) == 1) img[p.y][p.x].G = C[i];
      if ((alt % 3) == 2) img[p.y][p.x].B = C[i];
      alt++;
    }
    loc += 7;
  }

  // add terminating character (ETX)
  p = A.at(loc);
  x = p.x; y = p.y;
  if ((alt % 3) == 0) img[y][x].R = ETX;
  if ((alt % 3) == 1) img[y][x].G = ETX;
  if ((alt % 3) == 2) img[y][x].B = ETX;
  return;  
}


void decode(PPM &img, vector <pixel> &A) {
  unsigned char b7, b6, b5, b4, b3, b2, b1, b0;
  char a;  // store b7-b0 in this char 
  int i = 0;
  pixel p = A[i];
  int alt = 0;

  // loop through pixels in list
  while (1) {
    
    b0 = *((unsigned char *)&img[p.y][p.x] + alt);
    alt++; i++; p = A[i]; alt %= 3;
    if (b0 == ETX) break;
    b1 = *((unsigned char *)&img[p.y][p.x] + alt);
    alt++; i++; p = A[i]; alt %= 3;
    b2 = *((unsigned char *)&img[p.y][p.x] + alt);
    alt++; i++; p = A[i]; alt %= 3;
    b3 = *((unsigned char *)&img[p.y][p.x] + alt);
    alt++; i++; p = A[i]; alt %= 3;
    b4 = *((unsigned char *)&img[p.y][p.x] + alt);
    alt++; i++; p = A[i]; alt %= 3;
    b5 = *((unsigned char *)&img[p.y][p.x] + alt);
    alt++; i++; p = A[i]; alt %= 3;
    b6 = *((unsigned char *)&img[p.y][p.x] + alt);
    alt++; i++; p = A[i]; alt %= 3;
    b7 = 0x0;
    
    // put bits in charcter
    a = b0 | (b1 << 1) | (b2 << 2) | (b3 << 3) | (b4 << 4) 
           | (b5 << 5) | (b6 << 6) | (b7 << 7);
    
    // print character
    cout.put(a);
  }
  return;
}


int main(int argc, char *argv[]) {

  // parse command line arguments
  // if something not right,
  //   print error message and exit
  if (argc > 4 || argc < 3) { 
    cerr << "usage: Crypto -encode|-decode [ -option=value ] image.ppm\n";
    return -1;  
  } 
  string mode = "";
  string fname = "";
  string option = "";
  int seed; 
  for (int a = 0; a < argc; a++) {
    if (strcmp(argv[a], "./Crypto") == 0) continue;
    if (strcmp(argv[a], "-encode") == 0) { mode = "ENCODE"; continue; }
    if (strcmp(argv[a], "-decode") == 0) { mode = "DECODE"; continue; }
    if (strncmp(argv[a], "-seed=xx", 6) == 0) { option = argv[a]; continue; } 
    fname = argv[a];
  }
 
  if (mode == "") { 
    cerr << "usage: Crypto -encode|-decode [ -option=value ] image.ppm\n";
    return -1;
  }
   
  if (option != "" ) { 
    option.erase(0, 6);
    stringstream ss;
    ss << option;
    ss >> seed;
    ss.str(std::string());
  } else { seed = 0; }
 
  // set up PPM object and read image
  PPM img;
  img.read(fname);
 
  // declare and set vector of pixels
  vector <pixel> V;
  set_pixel_list(img, V, seed);
  
  if (mode == "ENCODE") {
    encode(img, V);
    img.write(fname);
  }
  else if (mode == "DECODE") { 
    decode(img, V);
  }
  return 0;
}
