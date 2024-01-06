#include "arc.h"
double TWO_PI = M_PI * 2;

vector<double> map_to_ellipse(vector<double> &coordinates, double rx, double ry, double cos_angle, double sin_angle, double center_x, double center_y)
{
    coordinates[0] *= rx;
    coordinates[1] *= ry;
    return {
        (cos_angle * coordinates[0] - sin_angle * coordinates[1]) + center_x,
        (sin_angle * coordinates[0] + cos_angle * coordinates[1]) + center_y};
}

vector<vector<double>> approximate_unit_arc(double this_angle, double that_angle)
{
    double a = (that_angle == 1.5707963267948966) ? 0.551915024494 : (that_angle == -1.5707963267948966) ? -0.551915024494
                                                                                                         : 4.0 / 3.0 * tan(that_angle / 4);
    double x1 = cos(this_angle);
    double y1 = sin(this_angle);
    double x2 = cos(this_angle + that_angle);
    double y2 = sin(this_angle + that_angle);
    return {
        {x1 - y1 * a, y1 + x1 * a},
        {x2 + y2 * a, y2 - x2 * a},
        {x2, y2}};
}

double vector_angle(double ux, double uy, double vx, double vy)
{
    int sign = (ux * vy - uy * vx < 0) ? -1 : 1;
    double dot = ux * vx + uy * vy;
    dot = (dot > 1) ? 1 : (dot < -1) ? -1
                                     : dot;
    return sign * acos(dot);
}

vector<double> get_arc_center(vector<double> &previous_point, double cx, double cy, double rx, double ry, int large_arc_flag, int sweep_flag, double sin_angle, double cos_angle, double pxp, double pyp)
{
    double rxsq = pow(rx, 2);
    double rysq = pow(ry, 2);
    double pxpsq = pow(pxp, 2);
    double pypsq = pow(pyp, 2);
    double radicant = (rxsq * rysq) - (rxsq * pypsq) - (rysq * pxpsq);
    if (radicant < 0)
        radicant = 0;
    radicant /= (rxsq * pypsq) + (rysq * pxpsq);
    radicant = sqrt(radicant) * (large_arc_flag == sweep_flag ? -1 : 1);
    double center_xp = radicant * rx / ry * pyp;
    double center_yp = radicant * -ry / rx * pxp;
    double center_x = cos_angle * center_xp - sin_angle * center_yp + (previous_point[0] + cx) / 2;
    double center_y = sin_angle * center_xp + cos_angle * center_yp + (previous_point[1] + cy) / 2;
    double vx1 = (pxp - center_xp) / rx;
    double vy1 = (pyp - center_yp) / ry;
    double vx2 = (-pxp - center_xp) / rx;
    double vy2 = (-pyp - center_yp) / ry;
    double this_angle = vector_angle(1, 0, vx1, vy1);
    double that_angle = vector_angle(vx1, vy1, vx2, vy2);
    if (sweep_flag == 0 && that_angle > 0)
        that_angle -= TWO_PI;
    if (sweep_flag == 1 && that_angle < 0)
        that_angle += TWO_PI;
    return {center_x, center_y, this_angle, that_angle};
}
