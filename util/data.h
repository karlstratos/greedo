// Author: Karl Stratos (stratos@cs.columbia.edu)
//
// A header file for the data class. An instance of this class
// represents the input data of word vectors. Each line of this data file
// corresponds to a distinct word type. There are two requirements
// to the formatting of this file.
//
// Requirement 1. Each line must have the following form:
//
//      frequency    word    value_1    value_2    ...    value_d
//
// where frequency is the count of the word type (in whatever corpus
// the vector is derived), word is the word type string, and value_1,
// value_2, ... value_d are the values of the entries of the vector
// for some d. Spacing of these attributes within the line doesn't matter.
//
// Requierment 2. The lines must be ordered in decreasing frequency.
// If your file contains n word types and looks like:
//      frequency_1 ...
//      frequency_2 ...
//      .
//      .
//      .
//      frequency_n ...
// then you must have frequency_1 >= frequency_2 >= ... >= frequency_n.

#ifndef DATA_H
#define DATA_H

#include <Eigen/Dense>
#include <vector>
#include <string>
#include <map>

using namespace std;


class data
{
    string path;  // path to the input file of word vectors
    
public:
    vector< Eigen::VectorXd > v;  // holder of vectors
    vector<string> x;  // holder of word types    
    vector<size_t> freq;  // holder of frequencies

    size_t n;  // number of distinct word types 
    size_t d;  // dimension of vectors

    // Builds an instance of the data class from the given file.
    void build(string datapath);
};


#endif  // DATA_H
