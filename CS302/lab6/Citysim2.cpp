#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <cstring>
using namespace std;

int citycount;

// haversine function
float haversine(float la1, float lo1, float la2, float lo2) {
   
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
  
  fout << "CITY INFO " << "(N=" << (int)A.size() << "):" 
       << "\n" << "\n";

  for (int i = 0; i < (int)A.size(); i++) {
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
    void create_citygraph(costtable &, vector<city> &);
    void write_citygraph(costtable &, vector<city> &);
    // void dijkstra_route(int, int, int, costtable &, vector<city> &);
  private:
    vector<float> table[2]; // 0 distance, 1 time
    vector<vector <int> > graph; 
    // typedef enum { WHITE, BLACK } vcolor_t;    
    // vector<vcolor_t> vcolor;
    // vector<float> vdist;
    // vector<int> vlink;
};

costtable::costtable() {
    int N = citycount;
    table[0].resize(N*(N+1)/2);
    table[1].resize(N*(N+1)/2);
    graph.assign(N, vector<int> (N, 0));
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

void costtable::create_citygraph(costtable &T, vector<city> &A) { 
  int N = citycount;
  float sav, sav1;
  int keep, keep1;
  for (int i = 0; i < N; i++) { 
    sav = 15000; sav1 = 5200;
    keep = -1; keep1 = -1;
    for (int j = 0; j < N; j++) {
      if (i == j) continue;
      // same zone
      if (A[i].get_zone() == A[j].get_zone()) {
        if (A[i].get_type() == A[j].get_type()) {
          T.graph[i][j] = 1;
          T.graph[j][i] = 1;
          continue;
        } else {
          if (T(0, i , j) < sav) { 
              sav = T(0, i, j);
              keep = j;
          }
        }
      // not same zone
      } else {
        sav1 = 5200; keep1 = -1;
        if (A[i].get_type() == "GATEWAY" && A[j].get_type() == "GATEWAY") {
          int p = A[j].get_zone();
          for (int k = 0; k < N; k++) { 
            if (A[k].get_zone() != p) continue;
            if (A[k].get_type() != "GATEWAY") continue;
              if (T(0, i, k) < sav1) {
                sav1 = T(0, i, k);
                keep1 = k;
              }
          }
          if (keep1 > -1) {
            T.graph[i][keep1] = 1;
            T.graph[keep1][i] = 1;
            continue;
          }
        }
      }
    }
    if (keep > -1) { 
      T.graph[i][keep] = 1;
      T.graph[keep][i] = 1;
      continue;
    }
  }
  return;
}

void costtable::write_citygraph(costtable &T, vector<city> &A) { 
  int N = citycount;
  ofstream fout;
  fout.open("city_graph.txt");
  fout << "CITY GRAPH:\n" << '\n';

  for (int i = 0; i < N; i++) {
    fout << setw(3) << right << i << ' ' << A[i].get_name() << '\n';
    for (int j = 0; j < N; j++) {
      if (T.graph[i][j] == 1) { 
        fout << setw(6) << right << j << ' '
             << setw(20) << left << A[j].get_name()
             << setw(6) << right << setprecision(1)
             << fixed << T(0,i,j) << " miles"
             << setw(6) << right << setprecision(1)
             << fixed << T(1,i,j) << " hours\n";
      }
    }
    fout << '\n';
  }  
  return;
}

/*
void costtable::dijkstra_route(int c, int source, int sink, costtable &T, vector<city> &A) { 
    int N = citycount;
    T.vcolor.assign(N, WHITE);  

    T.vdist.assign(N, numeric_limits<float>::max());  
    T.vdist[source] = 0;  

    T.vlink.assign(N, -1);
    T.vlink[source] = source;

  while (1) {
    int next_i = -1;
    float mindist = numeric_limits<float>::max();

    for (int i = 0; i < N; i++) {
      if (T.vcolor[i] == WHITE && mindist > T.vdist[i]) {
        next_i = i;
        mindist = T.vdist[i];
      }
    }
    
    int i = next_i;
    if (i == -1) return;

    T.vcolor[i] = BLACK;

    if (i == sink) break;
    
    for (int k = 0; k < N; k++) {      
      if (T.graph[i][k] == 0) continue; 
      float wij = T(c, i, k);      
        if (T.vcolor[k] == WHITE) {
          if (T.vdist[k] > T.vdist[i] + wij) {         
            T.vdist[k] = T.vdist[i] + wij;          
            T.vlink[k] = i;
          }      
        }
        cout << setw(7) << right << T(0, i, k) << " miles "
                 << setw(6) << right << T(1, i, k) << " hours : "
                 << setw(2) << right << i << ' ' << setw(19)
                 << left << A[i].get_name() << endl;    
    }
  }
  return;
}
*/

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
  T.create_distancetable(T, V);
  T.create_timetable(T, V);

  // create_citygraph()
  T.create_citygraph(T, V);

  if (mode == "-graphinfo") {
    write_cityinfo(V);

    T.write_traveldistance(T, V);
    T.write_traveltime(T, V);

    //write_citygraph()
    T.write_citygraph(T, V);
 
    //return from program
    return 0;
  } 
/*
  int costmode;
  if (mode == "-distance") costmode = 0;
  if (mode == "-time") costmode = 1;

  string from = ""; // source
  string to = "";   // sink
  int source, sink;

  while (1) {
	cout << "Enter> "; 
        cin >> from >> to;
        for (int a = 0; a < citycount; a++) {
          if (V[a].get_name() == from) source = a;
          if (V[a].get_name() == to) sink = a;
        }
    T.dijkstra_route(costmode, source, sink, T, V);
  }
*/
  return 0;
}
