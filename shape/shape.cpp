#include "shape.h"
#include <memory>
using namespace Shapes;

// Shape Class
Shape::Shape()
{
    this->stroke_width = 0;
    this->stroke_color = Color(255, 0, 0, 0);
    this->fill_color = Color(255, 0, 0, 0);
    this->transforms.clear();
}
Shape::Shape(Entity entity)
{
    if (entity.attributes.find("stroke") != entity.attributes.end())
        this->stroke_width = entity.attributes.find("stroke-width") != entity.attributes.end() ? stof(entity.attributes["stroke-width"]) : 1;
    else
        this->stroke_width = 0;

    this->stroke_color = Color("stroke", entity.attributes);
    this->fill_color = Color("fill", entity.attributes);
    this->fill_rule = (entity.attributes.find("fill-rule") != entity.attributes.end()) ? entity.attributes["fill-rule"] : "nonzero";

    if (entity.attributes.find("transform") != entity.attributes.end())
    {
        istringstream element_stream(entity.attributes["transform"]);
        string single_transform;
        while (getline(element_stream, single_transform, ')'))
        {
            Transform vessel;
            string values;
            float value;

            istringstream transform_stream(single_transform);
            getline(transform_stream, vessel.type, '(');
            getline(transform_stream, values);
            istringstream values_stream(values);
            while (values_stream >> value)
                vessel.values.push_back(value);
            vessel.type = format_text(vessel.type);
            this->transforms.push_back(vessel);
        }
    }
}
void Shape::apply_transform(Gdiplus::Graphics &graphics)
{
    for (const auto &transform : this->transforms)
    {
        if (transform.type == "translate")
            graphics.TranslateTransform(transform.values[0], transform.values[1]);
        else if (transform.type == "rotate")
            graphics.RotateTransform(transform.values[0]);
        else if (transform.type == "scale")
        {
            if (transform.values.size() > 1)
                graphics.ScaleTransform(transform.values[0], transform.values[1]);
            else
                graphics.ScaleTransform(transform.values[0], transform.values[0]);
        }
    }
}

// Rectangle Class
Rectangle::Rectangle(Entity entity) : Shape(entity)
{
    this->x = entity.attributes.find("x") != entity.attributes.end() ? stof(entity.attributes["x"]) : 0;
    this->y = entity.attributes.find("y") != entity.attributes.end() ? stof(entity.attributes["y"]) : 0;
    this->width = entity.attributes.find("width") != entity.attributes.end() ? stof(entity.attributes["width"]) : 0;
    this->height = entity.attributes.find("height") != entity.attributes.end() ? stof(entity.attributes["height"]) : 0;
}
void Rectangle::render(Gdiplus::Graphics &graphics)
{
    Gdiplus::GraphicsState state = graphics.Save();
    this->apply_transform(graphics);

    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(this->fill_color.alpha, this->fill_color.red, this->fill_color.green, this->fill_color.blue));
    Gdiplus::RectF shape(this->x, this->y, this->width, this->height);
    graphics.FillRectangle(&fill_pen, shape);

    if (this->stroke_width != 0)
    {
        Gdiplus::Pen stroke_pen(Gdiplus::Color(this->stroke_color.alpha, this->stroke_color.red, this->stroke_color.green, this->stroke_color.blue), this->stroke_width);
        graphics.DrawRectangle(&stroke_pen, shape);
    }
    graphics.Restore(state);
}

// Text Class
Text::Text(Entity entity) : Shape(entity)
{
    this->x = entity.attributes.find("x") != entity.attributes.end() ? stof(entity.attributes["x"]) : 0;
    this->y = entity.attributes.find("y") != entity.attributes.end() ? stof(entity.attributes["y"]) : 0;
    this->dx = entity.attributes.find("dx") != entity.attributes.end() ? stof(entity.attributes["dx"]) : 0;
    this->dy = entity.attributes.find("dy") != entity.attributes.end() ? stof(entity.attributes["dy"]) : 0;
    this->font_size = entity.attributes.find("font-size") != entity.attributes.end() ? stof(entity.attributes["font-size"]) : 0;
    this->font_family = entity.attributes.find("font-family") != entity.attributes.end() ? entity.attributes["font-family"] : "times new roman";
    this->text_anchor = entity.attributes.find("text-anchor") != entity.attributes.end() ? entity.attributes["text-anchor"] : "start";
    this->font_style = entity.attributes.find("font-style") != entity.attributes.end() ? entity.attributes["font-style"] : "normal";
    this->text_content = entity.text_content;
    offset_map = {
        {"start", -0.15 * this->font_size},
        {"middle", 0},
        {"end", 0.15 * this->font_size},
    };
}
void Text::render(Gdiplus::Graphics &graphics)
{
    Gdiplus::GraphicsState state = graphics.Save();
    this->apply_transform(graphics);

    wstring wide_content = wstring(this->text_content.begin(), this->text_content.end());
    wstring wide_font_family = wstring(this->font_family.begin(), this->font_family.end());

    unique_ptr<Gdiplus::FontFamily> font_family(new Gdiplus::FontFamily(wide_font_family.c_str()));
    
    Gdiplus::FontFamily font_family(wide_font_family.c_str());
    Gdiplus::FontStyle font_style = this->font_style_map[this->font_style];

    Gdiplus::PointF origin(this->x, this->y - 0.9 * this->font_size);
    origin.X += offset_map[this->text_anchor];

    Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericDefault());
    format.SetAlignment(this->text_anchor_map[this->text_anchor]);

    Gdiplus::GraphicsPath path;
    path.AddString(wide_content.c_str(), -1, &font_family, font_style, this->font_size, origin, &format);

    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(this->fill_color.alpha, this->fill_color.red, this->fill_color.green, this->fill_color.blue));
    graphics.FillPath(&fill_pen, &path);

    if (this->stroke_width != 0)
    {
        Gdiplus::Pen stroke_pen(Gdiplus::Color(this->stroke_color.alpha, this->stroke_color.red, this->stroke_color.green, this->stroke_color.blue), this->stroke_width);
        graphics.DrawPath(&stroke_pen, &path);
    }
    graphics.Restore(state);
}

// Ellipse Class
Ellipse::Ellipse(Entity entity) : Shape(entity)
{
    this->cx = entity.attributes.find("cx") != entity.attributes.end() ? stof(entity.attributes["cx"]) : 0;
    this->cy = entity.attributes.find("cy") != entity.attributes.end() ? stof(entity.attributes["cy"]) : 0;
    this->rx = entity.attributes.find("rx") != entity.attributes.end() ? stof(entity.attributes["rx"]) : 0;
    this->ry = entity.attributes.find("ry") != entity.attributes.end() ? stof(entity.attributes["ry"]) : 0;
}
void Ellipse::render(Gdiplus::Graphics &graphics)
{
    Gdiplus::GraphicsState state = graphics.Save();
    this->apply_transform(graphics);

    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(this->fill_color.alpha, this->fill_color.red, this->fill_color.green, this->fill_color.blue));
    Gdiplus::RectF shape(this->cx - this->rx, this->cy - this->ry, this->rx * 2, this->ry * 2);
    graphics.FillEllipse(&fill_pen, shape);

    if (this->stroke_width != 0)
    {
        Gdiplus::Pen stroke_pen(Gdiplus::Color(this->stroke_color.alpha, this->stroke_color.red, this->stroke_color.green, this->stroke_color.blue), this->stroke_width);
        graphics.DrawEllipse(&stroke_pen, shape);
    }
    graphics.Restore(state);
}

// Circle Class
Circle::Circle(Entity entity) : Ellipse(entity)
{
    this->rx = entity.attributes.find("r") != entity.attributes.end() ? stof(entity.attributes["r"]) : 0;
    this->ry = entity.attributes.find("r") != entity.attributes.end() ? stof(entity.attributes["r"]) : 0;
}

// Line Class
Line::Line(Entity entity) : Shape(entity)
{
    this->x1 = entity.attributes.find("x1") != entity.attributes.end() ? stof(entity.attributes["x1"]) : 0;
    this->y1 = entity.attributes.find("y1") != entity.attributes.end() ? stof(entity.attributes["y1"]) : 0;
    this->x2 = entity.attributes.find("x2") != entity.attributes.end() ? stof(entity.attributes["x2"]) : 0;
    this->y2 = entity.attributes.find("y2") != entity.attributes.end() ? stof(entity.attributes["y2"]) : 0;
}
void Line::render(Gdiplus::Graphics &graphics)
{
    Gdiplus::GraphicsState state = graphics.Save();
    this->apply_transform(graphics);

    Gdiplus::Pen stroke_pen(Gdiplus::Color(this->stroke_color.alpha, this->stroke_color.red, this->stroke_color.green, this->stroke_color.blue), this->stroke_width);
    graphics.DrawLine(&stroke_pen, this->x1, this->y1, this->x2, this->y2);
    graphics.Restore(state);
}

// PolyBase Class
PolyBase::PolyBase(Entity entity) : Shape(entity)
{
    stringstream point_stream(entity.attributes["points"]);
    float x, y;
    while (point_stream >> x)
    {
        point_stream >> y;
        this->points.push_back({x, y});
    }
}

// Polyline Class
Polyline::Polyline(Entity entity) : PolyBase(entity) {}
void Polyline::render(Gdiplus::Graphics &graphics)
{
    Gdiplus::GraphicsState state = graphics.Save();
    this->apply_transform(graphics);

    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(fill_color.alpha, fill_color.red, fill_color.green, fill_color.blue));
    Gdiplus::GraphicsPath path;
    for (size_t i = 0; i < points.size() - 1; i += 1)
        path.AddLine(points[i].first, points[i].second, points[i + 1].first, points[i + 1].second);
    graphics.FillPath(&fill_pen, &path);

    if (stroke_width != 0)
    {
        Gdiplus::Pen stroke_pen(Gdiplus::Color(stroke_color.alpha, stroke_color.red, stroke_color.green, stroke_color.blue), stroke_width);
        graphics.DrawPath(&stroke_pen, &path);
    }
    graphics.Restore(state);
}

// Polygon Class
Polygon::Polygon(Entity entity) : PolyBase(entity) {}
void Polygon::render(Gdiplus::Graphics &graphics)
{
    Gdiplus::GraphicsState state = graphics.Save();
    this->apply_transform(graphics);

    vector<Gdiplus::PointF> gdiplus_points;
    for (const auto &point : points)
        gdiplus_points.push_back(Gdiplus::PointF(point.first, point.second));

    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(fill_color.alpha, fill_color.red, fill_color.green, fill_color.blue));
    graphics.FillPolygon(&fill_pen, gdiplus_points.data(), gdiplus_points.size(), Gdiplus::FillModeWinding);

    if (stroke_width != 0)
    {
        Gdiplus::Pen stroke_pen(Gdiplus::Color(stroke_color.alpha, stroke_color.red, stroke_color.green, stroke_color.blue), stroke_width);
        graphics.DrawPolygon(&stroke_pen, gdiplus_points.data(), gdiplus_points.size());
    }
    graphics.Restore(state);
}

// Path Class
Path::Path(Entity entity) : Shape(entity)
{
}
void Path::render(Gdiplus::Graphics &graphics)
{
}
