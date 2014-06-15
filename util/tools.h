// Author: Karl Stratos (stratos@cs.columbia.edu)
//
// A catch-all header file for other useful tools.

#ifndef TOOLS_H
#define TOOLS_H

#include <vector>
#include <map>
#include <iostream>

using namespace std;

// closeness of two floats: this is used for comparing results with Matlab's.
// Since Matlab results are given up to 4 decimal places, we use the tolerance
// 1e-3 for comparison.
const double TINY = 1e-3;  


// Prints the remark at cout and exit(-1).
void report_fail(std::string remark);

// Returns true if |f1 - f2| < TINY.
bool very_close(double f1, double f2);


#endif // TOOLS_H
