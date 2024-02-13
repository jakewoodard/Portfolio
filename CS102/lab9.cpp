// Kenneth Woodard
// October 9th, 2019
// Dunn - Section 3
// TA: Angel
// Lab 9
// This program calculates the total of a speeding ticket based on the location of the incident and the recorded speed relative to the speed limit. It will output info to screen or output file based on the command line arguments.
// Help from Syd and Rach

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

// use global const for ticket fee multipliers
const double interstate = 5.2252;
const double highway = 9.4412;
const double residential = 17.1525;
const double none = 12.152;

// array to put months into 3 letter abbr
const string months[13] = {"none", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// date structure
struct Date {
    int year;
    int month;
    int day;
};

// ticket structure
struct Ticket {
    string citationNumber;
    Date c;
    int clockedSpeed;
    int speedLimit;
    char road;
};

// database class
class Database {
    // initialize vector to store each ticket
    vector<Ticket> tickets;
public:
    void add_ticket(Ticket &a);
    vector<Ticket> gen_report(int start_value, int end_value) const;
};

// standalone function prototypes
bool Read(Database &d, istream &in);
bool Write(const vector<Ticket> &tickets, ostream &out);
int Encode(const Date &d);


int main (string *argv[]) {
    
    // all the variables for file input info
    int start_value;
    int end_value;
    int MM1, MM2, DD1, DD2, YYYY1, YYYY2;
    
    // take command line arg 2 and 3 for start and end date
    stringstream ss(string argv[2]);
    ss >> YYYY1 >> MM1 >> DD1;
    Date s;
    s.year = YYYY1;
    s.month = MM1;
    s.day = DD1;
    start_value = Encode(s);
    ss.clear(); // clear stringstream
    stringstream ss(string argv[3]);
    ss >> YYYY2 >> MM2 >> DD2;
    Date e;
    e.year = YYYY2;
    e.month = MM2;
    e.day = DD2;
    end_value = Encode(e);
    
    // get file name from user and open file
    string filename;
    filename = argv[1];
    ifstream fin;
    fin.open(filename);
    
    // error check to make sure file can be opened
    if (!fin) {
        cout << "The file '" << filename << "' cannot be read.\n";
        return 1;
    }
    
    // read from file
    Database data;
    for (int i = 0; i < tickets.size; i++) {
    Read(data, fin);
        data.add_ticket(tickets.at(i));
    }
    
    // report file if argv4 != "-" and open it
    if (argv[4] != "-") {
        string report;
        report = argv[4];
        ofstream fout;
        fout.open(report);
    
        // error check to make sure output file can be opened
        if (!fout) {
            cout << "Unable to open " << report << ".\n";
        return 1;
        }
        Write(tickets, fout);
    }
    else {
        Write(tickets, cout);
    }
    return 0;
}


// function definitions
void Database::add_ticket(Ticket &a)  {
        tickets.push_back(a);
        }
    
int Encode(const date &d) {
        int value = (year * 10000) + (month * 100) + day;
        return value;
}
    
// generate report function creates new vector of tickets within range, and returns
vector<Ticket> Database::gen_report(int start, int end) {
    vector<Ticket> newtickets;
    for (int i = 0; i < tickets.size; i++) {
        int value = Encode(tickets.at(i).c);
        if (start <= value && end >= value) {
            add_ticket(tickets.at(i));
        }
    }
    return vector<Ticket> newtickets;
}

bool Read(Database &d, istream &in) {
    while (in >> citationNumber >> month >> day >> year >> clockedSpeed >> speedLimit >> road) {
    if (year < 100) {
        year = year + 2000;
    }
        
    if (!in) {
        return false;
    }
    else {
        return true;
    }
}
}
    
    
bool Write(const vector<Ticket> &tickets, ostream &out) {
        for (int i = 0; i < tickets.size; i++) {
            double fine;
            switch (road) {
                case 'I':
                case 'i': fine = (Ticket.clockedSpeed - Ticket.speedLimit) * interstate;
                    break;
                case 'R':
                case 'r': fine = (Ticket.clockedSpeed - Ticket.speedLimit) * residential;
                    break;
                case 'H':
                case 'h': fine = (Ticket.clockedSpeed - Ticket.speedLimit) * highway;
                    break;
                default: fine = (Ticket.clockedSpeed - Ticket.speedLimit) * none;
                    break;
            }
            // correct negative fine
            if (fine < 0) {
                fine = 0;
            }
            out << setfill('0') << setw(2) << right << tickets.at(i).c.day << left << "-" << setfill('0') << setw(2) << months[tickets.at(i).c.month] << "-" << tickets.at(i).c.year << setw(10) << setfill(' ') << tickets.at(i).citationNumber << "$" << setw(9) << setfill(' ') << right << setprecision(2) << fixed << fine << endl;
        }
    if (!out) {
        return false;
    }
    else {
        return true;
    }
}
