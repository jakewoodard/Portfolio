#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <cmath>
#include <cstring>
using namespace std;

int citycount;

// haversine function
float haversine(float la1, float lo1, float la2, float lo2) {
//  la1 = (la1) * 180.0 / (3.14159265);
//  la2 = (la2) * 180.0 / (3.14159265);
//  lo1 = (lo1) * 180.0 / (3.14159265);
//  lo2 = (lo2) * 180.0 / (3.14159265);
   
  // difference in lat and long
  float Dla = (la2 - la1) * (3.14159265) / 180.0;
  float Dlo = (lo2 - lo1) * (3.14159265) / 180.0;

  la1 = (la1) * (3.14159265) / 180.0;
  la2 = (la2) * (3.14159265) / 180.0;

  float y = sin(Dla/2.0);
        y = pow(y, 2.0);
  float x = sin(Dlo/2.0);
        x = pow(x, 2.0);
        x *= cos(la1);
        x *= cos(la2);
  float z = y + x;
        z = sqrt(z);
        z = asin(z);
        z *= 2;
  float earth = 3982.0;
        z *= earth;
        z = 10.0*round(z/10.0);
  
/*
  float a = (pow(sin(Dla / 2.0), 2.0)) + 
            (pow(sin(Dlo / 2.0), 2.0) * cos(la1) * cos(la2));
  float b = 2.0 * asin(sqrt(a));
        float earth = 3982.0;
        b *= earth;
        b = 10.0*round(b/10.0);
*/
  return z;
}

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
    din.latitude = (la) * (3.14159265/180.00);
    lo = din.longitude;
    din.longitude = (lo) * (3.14159265/180.00);
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
    A[i].latitude = (lat) * (180.00/3.14159265);
    A[i].longitude = (lon) * (180.00/3.14159265);
    fout << setw(3) << right << i << ' ' << A[i];
  }

  fout.close();
  return;
}

class costtable {
  public:
    costtable();
    float operator()(int, int, int);
    void create_distancetable(costtable &, vector<city> &);
    void create_timetable(costtable &, vector<city> &);
    void write_traveldistance(costtable &, vector<city> &);
    void write_traveltime(costtable &, vector<city> &);

  private:
    vector<float> table[2]; // 0 distance, 1 time
};

costtable::costtable() {
    int N = citycount;
    table[0].resize(N*(N+1)/2);
    table[1].resize(N*(N+1)/2);
}

float costtable::operator()(int m, int i, int j) {
  // swap
  if (j > i) {
    int temp = j;
    j = i; i = temp;
  }
  if (j == i) return 0.0;

  return table[m][i*(i+1)/2+j];
}

void costtable::create_distancetable(costtable &T, vector<city> &A) { 
  int N = citycount;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < i; j++) {
        float a = A[i].get_latitude();
        float s = A[i].get_longitude();
        float d = A[j].get_latitude();
        float f = A[j].get_longitude();
        T.table[0][i*(i+1)/2+j] = haversine(a, s, d, f);
    }
  }
  return;
}

void costtable::create_timetable(costtable &T, vector<city> &A) {
  int N = citycount;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < i; j++) {
      if (A[i].get_type() == "GATEWAY" && A[j].get_type() == "GATEWAY") {
        T.table[1][i*(i+1)/2+j] = T.table[0][i*(i+1)/2+j] / 570;
      } else
      if (A[i].get_type() == "REGIONAL" && A[j].get_type() == "REGIONAL") {
        T.table[1][i*(i+1)/2+j] = T.table[0][i*(i+1)/2+j] / 60;
      } else {
        T.table[1][i*(i+1)/2+j] = T.table[0][i*(i+1)/2+j] / 570;
      }
    }
  }
  return;
}

void costtable::write_traveldistance(costtable &T, vector<city> &A) {
  string a = "";
  int N = citycount;
  ofstream fout;
  fout.open("city_distancetable.txt");
  fout << "DISTANCE TABLE:\n";

  for(int i=0; i < N; i++) {
    for(int j=0; j < i; j++) {
      a += A[i].get_name();
      a += " to ";
      a += A[j].get_name();
      a += ' ';
      fout << setw(3) << right << i << ' '
           << setfill('.') << setw(40) << left << a << ' '
           << setfill(' ') << setw(7) << right << setprecision(1) << fixed
           << T(0, i, j) << setw(6) << right << " miles\n";
      a = "";
    }
    fout << '\n';
  }
  fout.close();
  return;
}

void costtable::write_traveltime(costtable &T, vector<city> &A) {
  string a = "";
  int N = citycount;
  ofstream fout;
  fout.open("city_timetable.txt");
  fout << "TIME TABLE:\n";

  for(int i=0; i < N; i++) {
    for(int j=0; j < i; j++) {
      a += A[i].get_name();
      a += " to ";
      a += A[j].get_name();
      a += ' ';
      fout << setw(3) << right << i << ' '
           << setfill('.') << setw(40) << left << a << ' '
           << setfill(' ') << setw(5) << right << setprecision(1) << fixed
           << T(1, i, j) << setw(6) << right << " hours\n";
      a = "";
    }
    fout << '\n';
  }
  fout.close();
  return;
}

//create_citygraph() { }
//write_citygraph() { }

//dijkstra_route() { }

int main(int argc, char *argv[]) {
  // commandline option decoding
  string mode;
  if (argc > 3 || argc < 2) { 
    cerr << "Usage: Citysim -graphinfo|-distance|-time [-seed=302|-seed=307]\n";
    return -1;
  }
  if (strcmp(argv[argc-1], "-graphinfo") == 0) { 
    mode = "-graphinfo";
  }
  // city graph declarations
  vector<city> V;

  // read_cityinfo()
  read_cityinfo(V);
  if (V.empty() == true) { cerr << "Error reading file\n"; return -1;}
  V.erase(V.begin());
  citycount = (int)V.size();

  //set up costtables
  costtable T;
  
  // create_citygraph()


  if (mode == "-graphinfo") {
    write_cityinfo(V);

    T.create_distancetable(T, V);
    T.create_timetable(T, V);
    T.write_traveldistance(T, V);
    T.write_traveltime(T, V);
	//write_citygraph()

	//return from program
        return 0;
  }
/*
  while (not done) {
	enter> from, to cities
    dijkstra_route(costmode,from,to) 
  }
*/
  return 0;
}
