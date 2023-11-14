#ifndef __COLOR__
#define __COLOR__
#include <string>
#include <sstream>
using namespace std;
class Color
{
public:
    int alpha, red, green, blue;
    Color();
    Color(int, int, int, int);
    Color(const string &, const string &);
    Color(const string &);
};
#endif
