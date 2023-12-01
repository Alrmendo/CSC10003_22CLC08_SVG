#ifndef __SHAPE__
#define __SHAPE__
#include <windows.h>
#include <gdiplus.h>
#include "color.h"
#include "../reader/read_file.h"
namespace Shapes
{
    struct Transform
    {
        string type;
        vector<float> values;
    };

    class Shape
    {
    protected:
        vector<Transform> transforms;
        Color fill_color;
        Color stroke_color;
        float stroke_width;

    public:
        Shape();
        Shape(Entity);
        void apply_transform(Gdiplus::Graphics &);
        virtual void render(Gdiplus::Graphics &) = 0;
    };

    class Rectangle : public Shape
    {
    public:
        float x, y, width, height;
        Rectangle();
        Rectangle(Entity);
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

    class Ellipse : public Shape
    {
    public:
        float cx, cy, rx, ry;
        Ellipse();
        Ellipse(Entity);
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

    class Polyline : public Shape
    {
    public:
        vector<pair<float, float>> points;
        Polyline();
        Polyline(Entity);
        void render_polyline(Gdiplus::Graphics &);
    };

    class Polygon : public Shape
    {
    public:
        vector<pair<float, float>> points;
        Polygon();
        Polygon(Entity);
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
}
#endif
