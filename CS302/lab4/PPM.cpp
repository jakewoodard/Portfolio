#include <sstream>
using namespace std;

#include "PPM.h"

void PPM::read(const string &fname) { 
  // buffers to read in P6, w, h, max
  string line1;
  string line2;
  string line3;
  
  // input file stream
  ifstream fin(fname, ios::binary); 

  // make sure file stream is open
  if (fin.is_open()) {
    getline(fin, line1);
  } else {
    cerr << "Error: unable to open file.\n";
    return;
  }

  // make sure file is of P6 type
  if (line1 != "P6") { 
    cerr << "Error: filename.ppm must be P6 type\n";
    return;
  }
  getline(fin, line2);

  // store header info and
  // make sure they arent zero
  stringstream ss;
  ss << line2;
  ss >> w >> h;
  ss.str(std::string());
  getline(fin, line3);
  ss << line3;
  ss >> max;
  ss.str(std::string());
  if (w == 0 || h == 0 || max == 0) return;
  
  // 3 b/c each pixel has 3 char values
  int j = 3 * w * h;
  img = new RGB*[h-1];
  for (int b = 0; b < w; b++) { 
    img[b] = new RGB[w-1]; 
  } 
  
  int a = 0;
  char * buffer = new char[j];

  // read file in
  fin.read(buffer, j);
  
  // store pixel data in img
  for (int d = 0; d < h; d++) { 
    for (int i = 0; i < w; i++) { 
      img[d][i].R = buffer[a++]; 
      img[d][i].G = buffer[a++];
      img[d][i].B = buffer[a++];
    }
  }
  fin.close();
  free(buffer);
  return;
}

void PPM::write(string &fname) { 
  // edit output file name
  string str = "_wmsg";
  fname.insert(fname.length() - 4, str);

  // output file stream
  ofstream fout(fname, ios::binary);

  // make sure stream is open
  if (!fout.is_open()) { 
    cerr << "Error: unable to open output file\n";
    return;
  }
  
  // output file header contents
  fout << "P6\n" << w << " " << h
       << "\n" << max << "\n";

  // total pixel values to write = i
  int k = 3 * w * h;
  int a = 0;
  char * buffer = new char[k];
 
  // store img pixel data into 
  // char buffer for writing
  for (int d = 0; d < h; d++) {
    for (int i = 0; i < w; i++) { 
      buffer[a++] = img[d][i].R;
      buffer[a++] = img[d][i].G;
      buffer[a++] = img[d][i].B;
    }
  }
 
  fout.write(buffer, k);
  fout.close();
  free(buffer);
  return;
}
