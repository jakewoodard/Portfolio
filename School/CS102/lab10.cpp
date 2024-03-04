// Kenneth Woodard
// November 20th, 2019
// Dunn - Section 3
// TA: Angel
// Lab 10
// Help from OH: Caleb Morgan, Rachel Fisher, TA: Spencer

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;

struct Pixel {
	int red;
    int green;
	int blue;
};

class Picture {
    vector<Pixel> pixels;
    int width;
    int height;
    int maxintensity;
public:
    Picture();
    const Pixel& get_pixel(int row, int column) const;
    Pixel& get_pixel(int row, int column);
    void set_pixel(int row, int column, const Pixel &p);
    bool read_input(istream &in);
    void write_output(ostream &out);
//  void remove_channel(const string &s);
};

int main(int argc, char *argv[]) {
    Picture Pic;
    
    string filename;
    filename = argv[1];
    string output;
    output = argv[2];
    
    if (filename == "-") {
        Pic.read_input(cin);
    }
    else {
        ifstream fin;
        fin.open(filename);
        Pic.read_input(fin);
        fin.close();
    }
    if (output == "-") {
        Pic.write_output(cout);
    }
    else {
        ofstream fout;
        fout.open(output);
        Pic.write_output(fout);
        fout.close();
    }

return 0;
}

Picture::Picture() {
    width = 0;
    height = 0;
    maxintensity = 0;
}

const Pixel& Picture::get_pixel(int row, int column) const {
    int placement;
    placement = ((row - 1)  * width) + (column - 1);
    return pixels.at(placement);
}

Pixel& Picture::get_pixel(int row, int column) {
    int placement;
    placement = ((row - 1)  * width) + (column - 1);
    return pixels.at(placement);
}

void Picture::set_pixel(int row, int column, const Pixel &p) {
    get_pixel(row, column) = p;
}

bool Picture::read_input(istream &in) {
    string line;
    bool line1 = false;
    bool line2 = false;
    bool line3 = false;
    bool check;
    pixels.reserve(width * height);
    while (getline(in, line)) {
        if (line == "P3") {
            line1 = true;
            continue;
        }
        else if (line[0] == '#') {
            continue;
        }
        else {
            if (line1 == true && line2 == false) {
                stringstream ss;
                ss << line;
                ss >> width >> height;
                line2 = true;
                continue;
            }
            else if (line1 == true && line2 == true && line3 == false) {
                line3 = true;
                stringstream ss;
                ss << line;
                ss >> maxintensity;
                continue;
            }
            else {
                Pixel p;
                stringstream ss;
                ss << line;
                while (ss >> p.red >> p.green >> p.blue) {
                    pixels.push_back(p);
                }
    }
    if (line1 == false) {
        cout << "Error. Wrong file type." << endl;
    }
    if (line1 == true && line2 == true && line3 == true) {
        check = true;
    }
    else {
        check = false;
    }
        }
    }
    return check;
}
    
void Picture::write_output(ostream &out) {
    out << "P3" << endl;
    out << "#width height" << endl;
    out << width << " " << height << endl;
    out << "#maximum intensity" << endl;
    out << maxintensity << endl;
    for (int i = 0; i < pixels.size(); i++) {
        out << pixels.at(i).red << " " << pixels.at(i).green << " " << pixels.at(i).blue << endl;
    }
}
/*
void Picture::remove_channel(const string &s) {
    if (s == "red") {
        
    }
    if (s == "green") {
        
    }
    if (s == "blue") {
        
    }
    if (s == "gray") {
        
    }
}
*/
