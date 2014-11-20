// Author: Karl Stratos (stratos@cs.columbia.edu)
//
// A header file for the program class. An instance of this class
// represents the program and takes care of reading the command line
// arguments and the input file, clustering, and writing results.

#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include <Eigen/Dense>

#include "data.h"
#include "str.h"
#include "tools.h"


using namespace std;
const int TAB = 40;  // TAB length for output display
const size_t STATUS_UNIT = 1000;  // report status every this many merges


class program {
    string output_dir;  // path to output directory
    ofstream logf;  // output_dir/"log"

public:
    string data_file;  // path to input data of word vectors
    vector< Eigen::VectorXd > v;  // holder of vectors
    vector<string> x;  // holder of word types
    vector<size_t> freq;  // holder of frequencies

    size_t n;  // number of distinct word types
    size_t m;  // number of active clusters
    size_t d;  // dimension of vectors
    vector< vector<double> > Z;  // output: record n-1 merges
    bool quiet;
    bool verbose;
    bool debug;

    program() { quiet = false; verbose = false; debug = false; }

    // Collects command line arguments.
    void get_args(int argc, char* argv[]);

    // Makes output directory.
    void mkdir();

    // Records the given remark to log and also cout if not quiet.
    void rec(string remark, bool newline = true);

    // Called at the start of logging.
    void start_logging();

    // Called at the end of logging.
    void end_logging();

    // Reads the input data of word vectors.
    void read_data();

    // Performs hierarchical clustering.
    void cluster();

    // Writes the clustering results to the output file.
    void write_bitstrings();

    // Transforms the hierarchy into a bit string format.
    void traverse(map<string, vector<size_t> >& subtree);

    // Compares the output with Matlab.
    void compare_matlab();
};


#endif  // PROGRAM_H
