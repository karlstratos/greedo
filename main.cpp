#include <stdlib.h>
#include "util/program.h"

int main (int argc, char* argv[])
{
    program P;

    P.get_args(argc, argv);

    P.start_logging();

    P.read_data();

    P.cluster();
    
    P.write_bitstrings();
    
    P.end_logging();

    if (P.debug) P.compare_matlab();

    return 0;
}
