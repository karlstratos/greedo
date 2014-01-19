#include <stdlib.h>
#include "util/program.h"

int main (int argc, char* argv[])
{
    program P;

    P.get_args(argc, argv);

    P.read_data();
    
    P.start_logging();

    P.cluster();

    P.end_logging();

    P.write_bitstrings();

    if (P.debug) P.compare_matlab();

    return 0;
}

