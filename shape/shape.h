#ifndef __SHAPE__
#define __SHAPE__
#include <windows.h>
#include <gdiplus.h>
#include "color.h"
#include "../reader/read_file.h"

class Shape
{
public:
    float stroke_width;
    Color stroke_color;
    Color fill_color;
    Shape();
    Shape(Entity);
};

class Rect : public Shape
{
public:
    float x, y, width, height;
    Rect();
    Rect(Entity);
    void render_rect(Gdiplus::Graphics &);
};

class Text : public Shape
{
public:
    float x, y;
    int font_size;
    string content, font_family;
    Text();
    Text(Entity);
    void render_text(Gdiplus::Graphics &);
};

class Elip : public Shape
{
public:
    float cx, cy, rx, ry;
    Elip();
    Elip(Entity);
    void render_ellipse(Gdiplus::Graphics &);
};

class Circle : public Shape
{
public:
    float cx, cy, r;
    Circle();
    Circle(Entity);
    void render_circle(Gdiplus::Graphics &);
};

class Line : public Shape
{
public:
    float x1, y1, x2, y2;
    Line();
    Line(Entity);
    void render_line(Gdiplus::Graphics &);
};

class Plyline : public Shape
{
public:
    vector<pair<float, float>> points;
    Plyline();
    Plyline(Entity);
    void render_polyline(Gdiplus::Graphics &);
};

class Plygon : public Shape
{
public:
    vector<pair<float, float>> points;
    Plygon();
    Plygon(Entity);
    void render_polygon(Gdiplus::Graphics &);
};

class Path : public Shape
{
public:
    vector<string> command = {"M", "L", "H", "V", "C", "S", "Q", "T", "A", "Z"};
    string path_data;
    Path();
    Path(Entity);
    void render_path(Gdiplus::Graphics &);
};
#endif