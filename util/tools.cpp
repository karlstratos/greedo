// Author: Karl Stratos (stratos@cs.columbia.edu)
//
// A .cpp file of tools.h.

#include "tools.h"

#include <math.h>
#include <iostream>
#include <fstream>
#include <cstdlib>


void report_fail(string remark) {
    cout << "Failure: " << remark << endl;
    exit(-1);
}

bool very_close(double f1, double f2) {
    return (f1 < f2 + TINY && f1 > f2 - TINY);
}
