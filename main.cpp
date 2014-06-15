// Author: Karl Stratos (stratos@cs.columbia.edu)
//
// An executable main file for hierarchical clustering of word vectors.
// See README for instructions on how to use it.
//
// Given n vectors corresponding to n distinct word types, it keeps
// a set of m <= n active clusters (initialized with the m most frequent
// word types) and merges a pair c, c' such that the cost (Ward Jr, 1963):
//       |c|*|c'|/(|c| + |c'|) ||center_c - center_{c'}||^2
// is the smallest, until there is a single hierarchy over all vectors.
// It uses the "lazy" tightening trick of Franti et al. (2000).
// For details, see:
//
// A spectral algorithm for learning class-based n-gram models of natural
// language (Stratos et al., 2014)

#include <stdlib.h>

#include "util/program.h"


int main (int argc, char* argv[]) {
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
