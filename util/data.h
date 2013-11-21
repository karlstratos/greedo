#ifndef DATA_H
#define DATA_H
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <map>

using namespace std;

class data
{
    string path;
public:
    size_t n; // number of points
    size_t d; // number of dimensions
    vector<size_t> freq; 
    vector<string> x;
    vector< Eigen::VectorXd > v;
    void build(string datapath);
};

#endif // DATA_H
