// Author: Karl Stratos (stratos@cs.columbia.edu)
//
// A header file for useful string methods, some of which were
// taken from https://github.com/percyliang/brown-cluster.

#ifndef STR_H
#define STR_H

#include <iostream>
#include <string>
#include <vector>
#include <stdarg.h>
#include <stdio.h>

using namespace std;


// "base.ext" -> "base"
string file_base(string s);

// "dir/file" -> "file"
string strip_dir(string s);

// "dir/file" -> "dir"
string get_dir(string s);

// Returns the string of printf results. E.g., ("%s %d", "foo", 3) -> "foo 3"
string str_printf(const char *fmt, ...);

// 3 -> "3"
string i_str(int x);

// 0.77 -> "0.77"
string f_str(double x);

// Splits the given line by delimiter into tokens.
void split(vector<string>& tokens, string line, string delimiter);

// "s" -> "s(i:j-1)"
string substr(const string &s, int i, int j);

// "s" -> "s(i:)"
string substr(const string &s, int i);


#endif  // STR_H
