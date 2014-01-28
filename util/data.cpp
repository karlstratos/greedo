#include "data.h"
#include "str.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

void data::build(string datapath)
{
    path = datapath;
    vector<string> toks;
    Eigen::VectorXd this_v;
    string line;
    ifstream datafile;
    size_t this_freq;
    size_t old_freq = 1e19;

    n = 0;
    d = 0;
    
    datafile.open(path.c_str());
    assert(datafile.is_open());
    
    while(datafile.good()) {
	getline(datafile, line);	    	    
	if(line == "") continue;

	toks.clear();
	split(toks, line, " "); // toks[0] = freq, toks[1] = word, toks[2:] = vector
	if (d == 0) {
	    d = toks.size() - 2;
	    this_v.resize(d);
	}

	this_freq = atoi(toks[0].c_str());
	if(this_freq > old_freq) std::cout << "assert violation at: " << toks[0] << " " << toks[1] << " (previous count: " << old_freq << ")" << std::endl;
	assert(this_freq <= old_freq);
	freq.push_back(this_freq);
	old_freq = this_freq;
	    
	x.push_back(toks[1]);
	for (size_t i = 0; i < d; i++) 
	    this_v(i) = atof(toks[2+i].c_str());
	v.push_back(this_v);
	n++;
    }
    datafile.close();
}
