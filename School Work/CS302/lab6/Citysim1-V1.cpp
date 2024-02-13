#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sstream>
using namespace std;

class city {
  public:
    city();
    string get_name() { return name; }
    string get_type() { return type; }
    int get_zone() { return zone; }
    float get_latitude() { return latitude; }
    float get_longitude() { return longitude; }
    int get_population() { return population; }
    friend void read_cityinfo(vector<city> &);
    friend void write_cityinfo(vector<city> &);
    friend istream & operator>>(istream &, city &);
    friend ostream & operator<<(ostream &, city &);

  private: 
    string name;
    string type; 
    int zone;
    float latitude;
    float longitude;
    int population;
};

city::city() {
  name = ""; type = "";
  zone = 0; population = 0;
  latitude = 0; longitude = 0;
}

istream & operator>>(istream &in, city &T) { 
  in >> T.zone >> T.name >> T.type 
     >> T.latitude >> T.longitude >> T.population;
  return in;
}

ostream & operator<<(ostream &out, city &c) {
  string n, t;
  int z, p;
  float la, lo;
  n = c.get_name();
  t = c.get_type();
  z = c.get_zone();
  p = c.get_population();
  la = c.get_latitude();
  lo = c.get_longitude();

  out << setw(18) << left << n 
      << setw(12) << left << t 
      << setw(2) << left << z
      << setw(10) << right << p 
      << setw(8) << right << setprecision(2) << fixed << la
      << setw(8) << right << setprecision(2) << fixed << lo
      << "\n";
  return out;
}

//class costtable;

void read_cityinfo(vector<city> &A) {
  char line[128];
  float lo, la;
  city din;
  ifstream fin;
  fin.open("city_list.txt");
 
  while (fin.getline(line, 128)) { 
    if (line[0] == '#') continue;
    if (line[0] == ' ') continue;
    if (line[0] == '\n') continue;
    stringstream ss;
    ss << line;
    ss >> din;
    la = din.latitude; 
    din.latitude = (la * (3.14159265/180));
    lo = din.longitude;
    din.longitude = (lo * (3.14159265/180));
    if (A.size() < 2) {
      A.push_back(din);
    } 
    else if (din.name != A.at(A.size() - 1).name) { 
      A.push_back(din);
    }
  }
  
  fin.close();
  return;
}

void write_cityinfo(vector<city> &A) {
  ofstream fout;
  fout.open("city_info.txt");
  float lat;
  float lon;
  
  fout << "CITY INFO " << "(N=" << (int)A.size() << "):" 
       << "\n" << "\n";

  for (int i = 0; i < (int)A.size(); i++) {
    lat = A[i].get_latitude();
    lon = A[i].get_longitude();
    A[i].latitude = (lat * (180/3.14159265));
    A[i].longitude = (lon * (180/3.14159265));
    fout << setw(3) << right << i << ' ' << A[i];
  }

  fout.close();
  return;
}

//write_distancetable() { }
//write_timetable() { }

//create_citygraph() { }
//write_citygraph() { }

//dijkstra_route() { }

int main(int argc, char *argv[])
{
  // commandline option decoding
  if (argc > 3 || argc < 2) { 
    cerr << "Usage: Citysim -graphinfo|-distance|-time [-seed=302|-seed=307]\n";
    return -1;
  }

  // city graph declarations
  vector<city> V;

  // read_cityinfo()
  read_cityinfo(V);
  if (V.empty() == true) {cerr << "failure\n"; return -1;}
  V.erase(V.begin());
  write_cityinfo(V);
  //set up costtables

  // create_citygraph()

/*
  if (mode == graphinfo) {
    write_cityinfo();

    write_distancetable()
    write_timetable()

	write_citygraph()

	return from program
  }

  while (not done) {
	enter> from, to cities
    dijkstra_route(costmode,from,to) 
  }
*/
  return 0;
}
