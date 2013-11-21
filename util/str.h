#ifndef STR_H
#define STR_H
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string file_base(string s);
string strip_dir(string s);
string get_dir(string s);
string str_printf(const char *fmt, ...);
string i_str(int x);
string f_str(double x);
void split(vector<string>& tokens, string line, string delimiter);
string substr(const string &s, int i, int j);
string substr(const string &s, int i);

#endif // STR_H
