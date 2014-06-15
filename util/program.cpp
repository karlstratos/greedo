// Author: Karl Stratos (stratos@cs.columbia.edu)
//
// A .cpp file for the program class.

#include "program.h"
#include <float.h>


void program::get_args(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
    {
	if(argv[i] == (string) "--data_file")
	    data_file = argv[++i];
	else if(argv[i] == (string) "--m") 
	    m = atoi(argv[++i]);
	else if(argv[i] == (string) "--out")
	    output_dir = argv[++i];
	else if(argv[i] == (string) "--quiet")
	    quiet = true;
	else if(argv[i] == (string) "--verbose")
	    verbose = true;	
	else if(argv[i] == (string) "--debug")
	    debug = true;
	else
	    report_fail(str_printf("Invalid argument (%s), run the command by itself to see the options.", argv[i]));
    }

    if (argc == 1)
    {
	cout << "Arguments:" << endl;
	cout << setw(TAB) << left << "--data_file {data}" << right << "path to data points" << endl;
	cout << setw(TAB) << left << "--m {#}" << right << "max number of active clusters" << endl;
	cout << setw(TAB) << left << "--out {dirpath}" << right << "store results in this directory" << endl;
	cout << setw(TAB) << left << "--quiet" << right << "no standard output" << endl;
	cout << setw(TAB) << left << "--verbose" << right << "track the algorithm (for debugging)" << endl;
	cout << setw(TAB) << left << "--debug" << right << "compare the result with matlab's" << endl;	
	exit(0);
    }
}


void program::mkdir()
{
    if (output_dir == "") {
	output_dir = "output/" + file_base(strip_dir(data_file));
	output_dir += ".m" + i_str(m);
	output_dir += ".out";
    }
    if (system(("mkdir -p " + output_dir).c_str()) != 0)
	report_fail("Can't create " + output_dir);
    if (system(("rm -f " + output_dir + "/*").c_str()) != 0)
	report_fail("Can't remove things in " + output_dir);    
}


void program::rec(string remark, bool newline)
{
    logf << remark;
    if (newline) logf << endl;

    if (!quiet)
    { 
	cout << remark;
	if (newline) cout << endl;
    }

    logf.flush();
    cout.flush();
}


time_t begin_time;

void program::start_logging()
{    
    mkdir();
    logf.open((output_dir + "/log").c_str());

    begin_time = time(NULL);
    rec(str_printf("Begin time: %s", ctime(&begin_time)));

    rec("-------------------------------------------------");
    rec("--data_file: \t" + data_file);
    rec("--n:         \t" + i_str(n));
    rec("--d:         \t" + i_str(d));
    rec("--m:         \t" + i_str(m));
    rec("--quiet:     \t" + i_str(quiet));
    rec("--verbose:   \t" + i_str(verbose));    
    rec("--debug:     \t" + i_str(debug));
    rec("-------------------------------------------------\n");
}


void program::end_logging()
{
    time_t end_time = time(NULL);
    rec(str_printf("End time: %s", ctime(&end_time)));
    rec(str_printf("Time taken: %.f seconds", difftime(end_time, begin_time)));
    logf.close();
}


void program::read_data()
{
    data D;
    rec("Reading data " + data_file);
    D.build(data_file);
    v = D.v;
    x = D.x;
    freq = D.freq;
    n = v.size();
    d = D.d;
    assert(m <= n);
    if (debug) assert(m == n);
}


// (static)  S contains sizes of clusters 0 ... 2n-2
vector<double>            S;     //      S[i] = size of cluster i 

// (dynamic) these are "holders" of m+1 active clusters 
vector<size_t>           I;      //      I[i] : cluster at position i
vector<Eigen::VectorXd>  C;      //      C[i] : center of I[i]
vector<double>           lb;     //     lb[i] : distance from I[i] to its twin
vector<size_t>        twin;      //   twin[i] : I[twin[i]] is thought to be the closest cluster to I[i]
vector<bool>         tight;      //  tight[i] : if this is true, lb[i] is tight and I[twin[i]] is definitely the closest cluster to I[i]


// take extreme care to ensure C[i] and S[I[i]] match for the following inline functions to be correct
inline double compute_dist(size_t i, size_t j) 
{
    Eigen::VectorXd diff = C[i] - C[j];
    double scale = 2 * S[I[i]] * S[I[j]] / (S[I[i]] + S[I[j]]);
    return scale * diff.squaredNorm(); 
}

inline Eigen::VectorXd merge(size_t i, size_t j) 
{
    double i_scale = S[I[i]] / (S[I[i]] + S[I[j]]);
    double j_scale = S[I[j]] / (S[I[i]] + S[I[j]]);
    return i_scale * C[i] + j_scale * C[j];
}


void program::cluster()
{
                                           // for i = 0...n-2    
    Z.resize(n - 1);                       //         Z[i][0] = index of the left child of cluster n+i
    for (size_t i = 0; i < n - 1; i++)     //         Z[i][1] = index of the right child of cluster n+i
	Z[i].resize(3);                    //         Z[i][2] = distance between the children of cluster n+i

    S.resize(2 * n - 1);                   
    I.resize(m+1);                         
    C.resize(m+1);
    lb.resize(m+1);
    twin.resize(m+1);
    tight.resize(m+1);

    // initialize the top m points as singleton clusters and compute pairwise distances: then tight[i] = true for i = 0 ... m-1
    for (size_t i = 0; i < m; i++)
    { 
	S[i] = 1; 
	I[i] = i;
	C[i] = v[i];
	lb[i] = DBL_MAX;
	for (int j = i-1; j >= 0; j--)
	{
	    double dist = compute_dist(i,j);
	    if (dist < lb[i]) { lb[i] = dist; twin[i] = j; tight[i] = true; }
	    if (dist < lb[j]) { lb[j] = dist; twin[j] = i; tight[j] = true; }
	}
    }

    if (verbose) { for (size_t i = 0; i < m; i++) rec(x[i] + " (" + i_str(i) + ") " + x[twin[i]] + " (" + i_str(twin[i]) + ") " + f_str(lb[i])); }

    // main loop: construct n - 1 hierarchical clusters i = n ... 2n-2
    size_t status = 0;
    size_t num_tightening = 0;
    for (size_t i = n; i < 2 * n - 1; i++) 
    {
	if (((size_t) i-n+1) % STATUS_UNIT == 0) { rec(str_printf("\r# of merges done: %d / %d", status, n-1), false);  status += STATUS_UNIT; }
	
	size_t t = i - n + m; // t ranges [m ... n-1] and also (don't care) [n ... n+m-2]

	// if t < n, set singleton cluster t as our (m+1)-th active cluster in our holders and compute distances to the other m active clusters
	if (t < n)
	{
	    S[t] = 1;
	    I[m] = t;
	    C[m] = v[t];
	    lb[m] = DBL_MAX;
	    for (size_t j = 0; j < m; j++)
	    {
		double dist = compute_dist(m, j);
		if (dist < lb[m]) { lb[m] = dist; twin[m] = j; tight[m] = true; }
		if (dist < lb[j]) { lb[j] = dist; twin[j] = m; tight[j] = true; } // lb[j] tight if this happens even if it wasn't before
	    }
	}
	
	size_t r = min(m + 1, 2 * n - i); // r ranges [m+1 ... 2]: only consider the first r elements in "holders"

	if (verbose) { cout << "I: "; for (size_t j = 0; j < r; j++) cout << I[j] << " "; cout << endl; }

	// find the cluster I[a] that has the (possibly underestimated) smallest twin distance 
	size_t a = 0;                         
	double min_dist = DBL_MAX;         
	for (size_t j = 0; j < r; j++) if (lb[j] < min_dist) { min_dist = lb[j]; a = j; }

	while (!tight[a])                 
	{
	    // I[a]'s twin distance was underestimated, so tighten it
	    num_tightening++;
	    lb[a] = DBL_MAX;
	    for (size_t j = 0; j < r; j++)
	    {
		if (j == a) continue;
		double dist = compute_dist(a, j);
		if (dist < lb[a]) { lb[a] = dist; twin[a] = j; tight[a] = true; }
		if (dist < lb[j]) { lb[j] = dist; twin[j] = a; tight[j] = true; } // lb[j] tight if this happens even if it wasn't before
	    }

	    // again, find the cluster I[a] that has the smallest twin distance (again, possibly underestimated)
	    min_dist = DBL_MAX;
	    for (size_t j = 0; j < r; j++) if (lb[j] < min_dist) { min_dist = lb[j]; a = j; }
	}
	
	size_t b = twin[a];
	if (a > b) { size_t temp = a; a = b; b = temp; } // now we have a < b

	if (verbose && I[a] < n && I[b] < n) rec("singletons:\t" + x[I[a]] + "\t" + x[I[b]]);

	// cluster whose twin was either I[a] or I[b] now has an underestimated twin distance since I[a] and I[b] are gone
	for (size_t j = 0; j < r; j++)
	    if (twin[j] == a || twin[j] == b) 
	    {
		tight[j] = false;
		if (verbose) rec("------" + i_str(j) + " (" + i_str(I[j]) + ")'s twin distance is loosened");
	    }

	// record the merge: cluster i is the merge of I[a] and I[b]
	if (verbose) rec("Merging " + i_str(I[a]) + " (size " + i_str(S[I[a]]) + ") and " + i_str(I[b]) + " (size " + i_str(S[I[b]]) + ")");
	Z[i-n][0] = I[a];           
	Z[i-n][1] = I[b];     
	Z[i-n][2] = min_dist;
	S[i] = S[I[a]] + S[I[b]];
	
	// put cluster i at position a in holders
	C[a] = merge(a, b); // MUST call merge(a, b) before changing I[a]!
	I[a] = i;                    
	lb[a] = DBL_MAX;
	for (size_t j = 0; j < r; j++)
	{
	    if (j == a || j == b) continue;
	    double dist = compute_dist(a, j);
	    if (dist < lb[a]) { lb[a] = dist; twin[a] = j; tight[a] = true; }
	    if (dist < lb[j]) { lb[j] = dist; twin[j] = a; tight[j] = true; } // lb[j] tight if this happens even if it wasn't before
	}

	// now shift the holder elements b+1 ... r-1 by one (overwriting b)
	for (size_t j = 0; j < r-1; j++)
	{
	    if (j < b && twin[j] > b) // even for non-shifting elements, we must shift their twin indices > b
		twin[j] = twin[j] - 1;

	    if (j >= b) // j ranges [b ... r-2] here: j points to cluster I[j+1]
	    {
		I[j] = I[j+1];   
		C[j] = C[j+1];   
		lb[j] = lb[j+1];
		(twin[j+1] < b) ? twin[j] = twin[j+1] : twin[j] = twin[j+1]-1;
		tight[j] = tight[j+1];
	    }

	    assert(j != twin[j]); // useful sanity check
	}
    }
    rec("\n---average number of tightnings per merge: " + f_str(((float) num_tightening) / ((float) n - 1)) + " (as opposed to m = " + i_str(m) + ")\n");
}


void program::compare_matlab()
{
    cout << "Will compare the result with matlab's" << endl;

    ofstream outf;
    outf.open("Z.mine.debug");
    for (size_t i = 0; i < Z.size(); i++)
	outf << Z[i][0]+1 << " " << Z[i][1]+1 << " " << sqrt(Z[i][2]) << endl;
    outf.close();
    
    outf.open("Y.debug");
    for (size_t i = 0; i < v.size(); i++)
	outf << v[i].transpose() << endl;
    outf.close();
    if (system(("/Applications/MATLAB_R2013b.app/bin/matlab -nodesktop -nosplash -nodisplay -r \"run('util/debug.m')\"")) != 0) // change path to matlab as needed
	report_fail("Can't run matlab---check if your path to matlab in program.cpp is correct");

    ifstream datafile;    
    datafile.open("Z.matlab.debug");
    assert(datafile.is_open());

    vector<string> toks;
    string line;
    size_t i = 0;
    bool same = true;
    while(datafile.good()) {
	getline(datafile, line);	    	    
	if(line == "") continue;

	toks.clear();
	split(toks, line, " "); // toks[0] = left, toks[1] = right, toks[2] = dist

	size_t left = atoi(toks[0].c_str()) - 1;
	size_t right = atoi(toks[1].c_str()) - 1;
	double dist = atof(toks[2].c_str());

	
	if (Z[i][0] != left || Z[i][1] != right || !very_close(sqrt(Z[i][2]), dist))
	{
	    cout << "row " << i+1 << " not the same!" << endl;
	    cout << "matlab: " << left+1    << " " << right+1   << " " << dist << endl;
	    cout << "this:   " << Z[i][0]+1 << " " << Z[i][1]+1 << " " << sqrt(Z[i][2]) << endl << endl;
	    same = false;
	}
	i++;
    }
    datafile.close();

    if (same) cout << "Our Z is exactly the same as matlab's Z" << endl;
}


std::vector<size_t> lst; 
bool lst_compare(size_t i, size_t j)
{
  return lst[i] > lst[j];
}


void program::write_bitstrings()
{
    for (size_t i = 0; i < n-1; i++)
	if (Z[i][0] > Z[i][1])
	{
	    double temp = Z[i][0]; Z[i][0] = Z[i][1]; Z[i][1] = temp;
	}
    
    rec("Encoding clusters as bit strings to " + output_dir);
    map<string, vector<size_t> > subtree;       // subtree[001011] = { words under subtree 001011 }
    traverse("", 2*n-2, subtree);               // root cluster: 2n-2
    
    ofstream outf;
    outf.open((output_dir + "/paths").c_str());
    for(map<string, vector<size_t> >::iterator itr = subtree.begin(); itr != subtree.end(); itr++)
    {
	lst.clear();
	vector<size_t> words = itr->second;
	vector<size_t> inds;
	for (size_t i = 0; i < words.size(); i++)
	{
	    lst.push_back(freq[words[i]]);
	    inds.push_back(i);
	}
	
	sort(inds.begin(), inds.end(), lst_compare); // sort indices by the word frequncies 

	for (size_t i = 0; i < lst.size(); i++)
	    outf << itr->first << "\t" << x[words[inds[i]]] << "\t" << freq[words[inds[i]]] << endl;
    }
    outf.close();    
}


// Recursively traverse the hierarchy to label every leaf node with a bit string
// indicating the path from the root.
void program::traverse (string bits, size_t node, map<string, vector<size_t> >& subtree)
{
    // if node < n, it's a leaf node 
    if (node < n) 
	subtree[bits].push_back(node);

    else
    {
	size_t node1 = Z[node-n][0];
	size_t node2 = Z[node-n][1];

	string left_bits = bits;
	string right_bits = bits;
	
	if (node >= 2*n - m)
	{
	    left_bits = left_bits + "0";
	    right_bits = right_bits + "1";
	}

	traverse(left_bits,  node1, subtree);
	traverse(right_bits, node2, subtree);
    }
}
