#include "str.h"
#include <stdarg.h>
#include <stdio.h>

string file_base(string s) // "base.ext" -> "base"
{ 
    int i = s.rfind('.');
    return i == -1 ? s : substr(s, 0, i);
}

string strip_dir(string s) // "dir/file" -> "file"
{
    return substr(s, s.rfind('/')+1);    
}

string get_dir(string s)   // "dir/file" -> "dir"
{
    int i = s.rfind('/');
    return i == -1 ? "." : substr(s, 0, s.rfind('/'));
}

void split(vector<string> & tokens, string line, string delimiter)
{
    size_t start = 0, end = 0;
    string token;
    while (end != string::npos)
    {
	end = line.find(delimiter, start);
	if(end == string::npos)
	    token = line.substr(start, string::npos);
	else
	    token = line.substr(start, end - start);
      
	if(token != "") 
	    tokens.push_back(token);

	if(end > string::npos - delimiter.size())
	    start = string::npos;
	else
	    start = end + delimiter.size();
    }
}

string str_printf(const char *fmt, ...)
{
  char buf[16384];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  return buf;
}

string i_str(int x)
{
    return str_printf("%d", x);  
}

string f_str(double x)
{
    return str_printf("%f", x);
}

string substr(const string &s, int i, int j) // "s" -> "s(i:j-1)"
{
    if(i < 0) i += s.length();
    if(j < 0) j += s.length();
    i = max(i, 0);
    j = max(j, i);
    return s.substr(i, j-i);
}

string substr(const string &s, int i)       // "s" -> "s(i:)"
{
    return substr(s, i, s.length());
}
