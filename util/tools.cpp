#include "tools.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
const double TINY = 1e-3;

void report_fail(string remark)
{
    cout << "Failure: " << remark << endl;
    exit(-1);
}

bool very_close(double f1, double f2)
{
    return (f1 < f2 + TINY && f1 > f2 - TINY);
}
