#ifndef __COLOR__
#define __COLOR__

#include <string>
#include <sstream>
#include <unordered_map>
using namespace std;

class Color
{
public:
    int alpha, red, green, blue;
    Color();
    Color(int, int, int);
    Color(int, int, int, int);
    Color(const string &, const string &);
    Color(const string &);
    Color(const string &, unordered_map<string, string> &);
    void hex_to_rgb(const string &);
};
#endif
