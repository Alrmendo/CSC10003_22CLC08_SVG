#ifndef __ARC__
#define __ARC__
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
using namespace std;

vector<double> map_to_ellipse(vector<double> &, double, double, double, double, double, double);

vector<vector<double>> approximate_unit_arc(double, double);

double vector_angle(double, double, double, double);

vector<double> get_arc_center(vector<double> &, double, double, double, double, int, int, double, double, double, double);

vector<vector<vector<double>>> arc_to_cubic_beziers(vector<double> &, double, double, double, int, int, double, double);
#endif
