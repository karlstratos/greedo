#ifndef PROGRAM_H
#define PROGRAM_H
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include "data.h"
#include "str.h"
#include "tools.h"

const int TAB = 40;
using namespace std;

class program
{
    string output_dir;
    ofstream logf;
public:
    string data_file;
    size_t n;    
    size_t m;
    size_t d;     
    bool quiet;
    bool verbose;
    bool debug;

    vector< vector<double> > Z; // output: record n-1 merges
    vector< Eigen::VectorXd > v;
    vector<string> x;
    vector<size_t> freq;
    
    program() { quiet = false; verbose = false; debug = false; }

    void get_args(int argc, char* argv[]);
    void mkdir();
    void rec(string remark, bool newline = true);
    void start_logging(); 
    void end_logging();
    void read_data();
    
    void cluster();
    void write_bitstrings();
    void traverse (string bits, size_t node, map<string, vector<size_t> >& subtree);
    void compare_matlab();
};

#endif // PROGRAM_H
