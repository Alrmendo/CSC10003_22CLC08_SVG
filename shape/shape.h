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
        string fill_rule;
        unordered_map<string, Gdiplus::FillMode> fill_rule_map = {
            {"nonzero", Gdiplus::FillModeWinding},
            {"evenodd", Gdiplus::FillModeAlternate},
        };

    public:
        Shape();
        Shape(Entity);
        void apply_transform(Gdiplus::Graphics &);
        virtual void render(Gdiplus::Graphics &) = 0;
    };

    class Rectangle : public Shape
    {
    protected:
        float x, y, width, height;

    public:
        Rectangle();
        Rectangle(Entity);
        void render(Gdiplus::Graphics &) override;
    };

    class Text : public Shape
    {
    protected:
        float x, y, dx, dy, font_size;
        unordered_map<string, Gdiplus::FontStyle> font_style_map = {
            {"normal", Gdiplus::FontStyleRegular},
            {"italic", Gdiplus::FontStyleItalic},
        };
        unordered_map<string, Gdiplus::StringAlignment> text_anchor_map = {
            {"start", Gdiplus::StringAlignment::StringAlignmentNear},
            {"middle", Gdiplus::StringAlignment::StringAlignmentCenter},
            {"end", Gdiplus::StringAlignment::StringAlignmentFar},
        };
        unordered_map<string, float> offset_map;
        string text_content, font_family, text_anchor, font_style;

    public:
        Text();
        Text(Entity);
        void render(Gdiplus::Graphics &) override;
    };

    class Ellipse : public Shape
    {
    protected:
        float cx, cy, rx, ry;

    public:
        Ellipse();
        Ellipse(Entity);
        void render(Gdiplus::Graphics &) override;
    };

    class Circle : public Ellipse
    {
    public:
        Circle();
        Circle(Entity);
    };

    class Line : public Shape
    {
    protected:
        float x1, y1, x2, y2;

    public:
        Line();
        Line(Entity);
        void render(Gdiplus::Graphics &) override;
    };

    class PolyBase : public Shape
    {
    protected:
        vector<pair<float, float>> points;

    public:
        PolyBase();
        PolyBase(Entity);
    };

    class Polyline : public PolyBase
    {
    public:
        Polyline();
        Polyline(Entity);
        void render(Gdiplus::Graphics &) override;
    };

    class Polygon : public PolyBase
    {
    public:
        Polygon();
        Polygon(Entity);
        void render(Gdiplus::Graphics &) override;
    };

    struct PathSegment
    {
        char type;
        vector<float> points;
    };

    class Path : public Shape
    {
    public:
        Path();
        Path(Entity);
        void render(Gdiplus::Graphics &) override;
    };
}
#endif
