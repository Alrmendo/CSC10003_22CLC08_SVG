#include "shape.h"
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

Rectangle::Rectangle(Entity entity) : Shape(entity)
{
    this->x = stof(entity.attributes["x"]);
    this->y = stof(entity.attributes["y"]);
    this->width = stof(entity.attributes["width"]);
    this->height = stof(entity.attributes["height"]);
}
void Rectangle::render_rect(Gdiplus::Graphics &graphics)
{
    Gdiplus::Pen stroke_pen(Gdiplus::Color(this->stroke_color.alpha, this->stroke_color.red, this->stroke_color.green, this->stroke_color.blue), this->stroke_width);
    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(this->fill_color.alpha, this->fill_color.red, this->fill_color.green, this->fill_color.blue));
    Gdiplus::RectF shape(this->x, this->y, this->width, this->height);
    graphics.FillRectangle(&fill_pen, shape);
    if (this->stroke_width != 0)
        graphics.DrawRectangle(&stroke_pen, shape);
}

Text::Text(Entity entity) : Shape(entity)
{
    this->x = stof(entity.attributes["x"]);
    this->y = stof(entity.attributes["y"]);
    this->font_size = stoi(entity.attributes["font-size"]);
    if (!this->font_family.empty())
        this->font_family = entity.attributes["font-family"];
    else
        this->font_family = "Times New Roman";
    this->content = entity.content;
}
void Text::render_text(Gdiplus::Graphics &graphics)
{
    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(this->fill_color.alpha, this->fill_color.red, this->fill_color.green, this->fill_color.blue));
    wstring wide_font_family(this->font_family.begin(), this->font_family.end());
    Gdiplus::FontFamily font_family(wide_font_family.c_str());
    Gdiplus::Font font(&font_family, this->font_size, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::PointF point_f(this->x, this->y - this->font_size);
    wstring wide_content(this->content.begin(), this->content.end());
    graphics.DrawString(wide_content.c_str(), -1, &font, point_f, &fill_pen);
}

Ellipse::Ellipse(Entity entity) : Shape(entity)
{
    this->cx = stof(entity.attributes["cx"]);
    this->cy = stof(entity.attributes["cy"]);
    this->rx = stof(entity.attributes["rx"]);
    this->ry = stof(entity.attributes["ry"]);
}
void Ellipse::render_ellipse(Gdiplus::Graphics &graphics)
{
    Gdiplus::Pen stroke_pen(Gdiplus::Color(this->stroke_color.alpha, this->stroke_color.red, this->stroke_color.green, this->stroke_color.blue), this->stroke_width);
    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(this->fill_color.alpha, this->fill_color.red, this->fill_color.green, this->fill_color.blue));
    Gdiplus::RectF shape(this->cx - this->rx, this->cy - this->ry, this->rx * 2, this->ry * 2);
    graphics.FillEllipse(&fill_pen, shape);
    if (this->stroke_width != 0)
        graphics.DrawEllipse(&stroke_pen, shape);
}

Circle::Circle(Entity entity) : Shape(entity)
{
    this->cx = stof(entity.attributes["cx"]);
    this->cy = stof(entity.attributes["cy"]);
    this->r = stof(entity.attributes["r"]);
}
void Circle::render_circle(Gdiplus::Graphics &graphics)
{
    Gdiplus::Pen stroke_pen(Gdiplus::Color(this->stroke_color.alpha, this->stroke_color.red, this->stroke_color.green, this->stroke_color.blue), this->stroke_width);
    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(this->fill_color.alpha, this->fill_color.red, this->fill_color.green, this->fill_color.blue));
    Gdiplus::RectF shape(this->cx - this->r, this->cy - this->r, this->r * 2, this->r * 2);
    graphics.FillEllipse(&fill_pen, shape);
    if (this->stroke_width != 0)
        graphics.DrawEllipse(&stroke_pen, shape);
}

Line::Line(Entity entity) : Shape(entity)
{
    this->x1 = stof(entity.attributes["x1"]);
    this->y1 = stof(entity.attributes["y1"]);
    this->x2 = stof(entity.attributes["x2"]);
    this->y2 = stof(entity.attributes["y2"]);
}
void Line::render_line(Gdiplus::Graphics &graphics)
{
    Gdiplus::Pen stroke_pen(Gdiplus::Color(this->stroke_color.alpha, this->stroke_color.red, this->stroke_color.green, this->stroke_color.blue), this->stroke_width);
    graphics.DrawLine(&stroke_pen, this->x1, this->y1, this->x2, this->y2);
}

Polyline::Polyline(Entity entity) : Shape(entity)
{
    stringstream ss(entity.attributes["points"]);
    string point;

    while (getline(ss, point, ' '))
    {
        size_t comma_postion = point.find(',');
        if (comma_postion != string::npos)
        {
            float x = stof(point.substr(0, comma_postion));
            float y = stof(point.substr(comma_postion + 1));
            this->points.push_back({x, y});
        }
    }
}
void Polyline::render_polyline(Gdiplus::Graphics &graphics)
{
    Gdiplus::Pen stroke_pen(Gdiplus::Color(this->stroke_color.alpha, this->stroke_color.red, this->stroke_color.green, this->stroke_color.blue), this->stroke_width);
    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(this->fill_color.alpha, this->fill_color.red, this->fill_color.green, this->fill_color.blue));
    Gdiplus::GraphicsPath path;
    for (size_t i = 0; i < this->points.size() - 1; i += 1)
        path.AddLine(this->points[i].first, this->points[i].second, this->points[i + 1].first, this->points[i + 1].second);
    graphics.FillPath(&fill_pen, &path);
    if (this->stroke_width != 0)
        graphics.DrawPath(&stroke_pen, &path);
}

Polygon::Polygon(Entity entity) : Shape(entity)
{
    stringstream ss(entity.attributes["points"]);
    string point;

    while (getline(ss, point, ' '))
    {
        size_t comma_postion = point.find(',');
        if (comma_postion != string::npos)
        {
            float x = stof(point.substr(0, comma_postion));
            float y = stof(point.substr(comma_postion + 1));
            this->points.push_back({x, y});
        }
    }
}
void Polygon::render_polygon(Gdiplus::Graphics &graphics)
{
    Gdiplus::Pen stroke_pen(Gdiplus::Color(this->stroke_color.alpha, this->stroke_color.red, this->stroke_color.green, this->stroke_color.blue), this->stroke_width);
    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(this->fill_color.alpha, this->fill_color.red, this->fill_color.green, this->fill_color.blue));
    vector<Gdiplus::PointF> points;
    for (const auto &point : this->points)
        points.push_back(Gdiplus::PointF(point.first, point.second));
    graphics.FillPolygon(&fill_pen, points.data(), points.size());
    if (this->stroke_width != 0)
        graphics.DrawPolygon(&stroke_pen, points.data(), points.size());
}
