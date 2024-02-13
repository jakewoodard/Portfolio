#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cstdlib>
using namespace std;

typedef unsigned int uint;

class hash_table {  
	public:    
		hash_table(int N);
		int hash(const string key);
                int nextprime(int N);
                int qprobe(const string key, bool x);
                void insert(const string key, int linenum);
		void resize();
		void insert_done() { showload(); }
		void set_showstats(bool b) { b = true; }
		const vector<int>& find(string &key); 

	private:
		struct key_line
		{
		   string key;
		   vector<int> lines;

		   bool inuse() { return key.empty(); }
		   bool operator==(const string s) { return s == key; }
		};   
		
		vector<key_line> table;
		void showload();

	public:
		int num_inuse;
		int max_inuse;
		int collisions;
};

// constructor
hash_table::hash_table(int N) {  
	N = nextprime(N);
	table.assign(N, key_line());   
	
	num_inuse = 0;
	max_inuse = N/2; // quadratic probe max value
	collisions = 0;
}

int hash_table::hash(const string key) {
	uint index = 0;
	const char *c = key.c_str();  
	while (*c)    
		index = ((index << 5) | (index >> 27)) + *c++;  
	return index % table.size();
}

void hash_table::insert(const string key, int linenum) {  
    // check to make sure key is not already in table

    int num = qprobe(key, true);
    	
    if(!table[num].inuse()) {
	table[num].key = key;
	num_inuse += 1;
    }
	// iterator to search int vector for table[num]
	vector<int>::iterator it;
        it = std::find(table[num].lines.begin(), table[num].lines.end(),linenum);
	
	// if num is new then insert
	if (it == table[num].lines.end())
	     table[num].lines.push_back(num);

	if(num_inuse >= max_inuse) { insert_done(); resize(); }
}

int hash_table::nextprime(int N) {
	int i = 2;
	N = N * 2 + 1;
	while (i*i <= N) {    
		if (N % i == 0) { N = N + 2; i = 2; }    
		i++;  
	}  
	return max(3, N);
}

int hash_table::qprobe(const string key, bool x) {
	int i = hash(key);
	int k = 0;
	while (table[i].inuse() && !(table[i] == key)) {    
		if (x == true) collisions++;
		i += 2*(++k) - 1;    
		i = i % table.size();
  	}
	return i;
}

void hash_table::resize() {
	vector<key_line> tmp_table;
	for (int i = 0; i<(int)table.size(); i++) {    
		if (table[i].inuse() == true) tmp_table.push_back(table[i]);  
	}  
	
	int N = nextprime(table.size());
	table.assign(N, key_line());
	num_inuse = 0;
	max_inuse = N/2;

	for (int i = 0; i<(int)tmp_table.size(); i++) {    
		key_line &k = tmp_table[i];
		table[qprobe(k.key, false)] = k;
		num_inuse++;
	}
}

void hash_table::showload() {
	double num = num_inuse;
	double sizet = table.size();
	double ratio = num / sizet;
	cout << "** N =" << setw(7) << right << table.size()
	     << " load = " << setprecision(2) << ratio << endl;
}

const vector<int>& hash_table::find(string &key) {
	return table[qprobe(key, false)].lines;
}

char replace_punctuation(char c) { if (ispunct(c)) return ' '; return c; }

int main(int argc, char *argv[]) {
	
	// filestream and filename string
	// int j to increment through command line arguments
	ifstream fin;
	int j = 1;
	bool stats, stats1 = false; // bool variables for set_showstats()
	string filename;
	int N = 23; // default N value
	
	// parsing of command line arguments
	while (j < argc - 1) {
	    if (strcmp("-f", argv[j]) == 0) { filename = argv[j+1]; }
	    if (strcmp("-N", argv[j]) == 0) { N = atoi(argv[j+1]); }
	    if (strcmp("-showstats", argv[j]) == 0) { stats = true; }
	    j++;	
	}
	
	// error checking for negative N
	if (N < 1) {
		cerr << "error: enter value greater than 0\n";
		return -1;
	}

	// hashtable class object
        hash_table H(N);
        if (stats == true) H.set_showstats(stats1);

	// vector of strings to store lines from file
	// one with and one without puncuation
	vector<string> v;

	// open file and read in
	fin.open(filename);
	string str; // place holder string variable
	istringstream ss;
	
	int line = 1;
	while (getline(fin, str)) {
		v.push_back(str);
		// call transform to remove punctuation from each line
		transform(str.begin(), str.end(), str.begin(), replace_punctuation);
		// string stream to separate words
		ss.clear(); // clear stringstream
		ss.str(); // add new value
		string word; // individual word variable
		while(ss >> word) H.insert(word, line); // insert into table
		line++; // increment line number
	}
	
        // call insert_done once done w input file
        H.insert_done();
	
	cout << v.size() << endl;

	// enter loop for user to search
	cout << "find> ";
	vector<int> linenum;
	string s;
	while (cin >> s) {
		linenum = H.find(s);
		for (int i = 0; i < linenum.size(); i++) {
			cout << linenum.at(i) << ":  " << v.at(linenum.at(i) - 1) << endl;
		}
		cout << "find> ";
	}
	
	// close file when done
        if (fin.eof()) fin.close();

	// condition for showstats
	if (stats1 == true) {
		cout << "Run stats...\n"
		     << "Number of slots used: "
		     << H.num_inuse << endl
		     << "Max number of slots:  "
		     << H.max_inuse << endl
		     << "Number of collisions: "
		     << H.collisions << endl;
	}
return 0;
}

