#include "shape.h"
Shape::Shape()
{
    this->stroke_width = 0;
    this->stroke_color = Color(255, 0, 0, 0);
    this->fill_color = Color(255, 0, 0, 0);
}
Shape::Shape(Entity entity)
{
    if (!entity.attributes["stroke-width"].empty())
        this->stroke_width = stof(entity.attributes["stroke-width"]);
    else
        this->stroke_width = 0;
    this->stroke_color = Color(entity.attributes["stroke-opacity"], entity.attributes["stroke"]);
    this->fill_color = Color(entity.attributes["fill-opacity"], entity.attributes["fill"]);
}

Rect::Rect(Entity entity) : Shape(entity)
{
    this->x = stof(entity.attributes["x"]);
    this->y = stof(entity.attributes["y"]);
    this->width = stof(entity.attributes["width"]);
    this->height = stof(entity.attributes["height"]);
}
void Rect::render_rect(Gdiplus::Graphics &graphics)
{
    Gdiplus::Pen stroke_pen(Gdiplus::Color(this->stroke_color.alpha, this->stroke_color.red, this->stroke_color.green, this->stroke_color.blue), this->stroke_width);
    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(this->fill_color.alpha, this->fill_color.red, this->fill_color.green, this->fill_color.blue));
    Gdiplus::RectF shape(this->x, this->y, this->width, this->height);
    graphics.FillRectangle(&fill_pen, shape);
    graphics.DrawRectangle(&stroke_pen, shape);
}

Text::Text(Entity entity) : Shape(entity)
{
    this->x = stof(entity.attributes["x"]);
    this->y = stof(entity.attributes["y"]);
    this->font_size = stoi(entity.attributes["font-size"]);
    this->content = entity.content;
}
void Text::render_text(Gdiplus::Graphics &graphics)
{
    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(this->fill_color.alpha, this->fill_color.red, this->fill_color.green, this->fill_color.blue));
    Gdiplus::FontFamily font_family(L"Times New Roman");
    Gdiplus::Font font(&font_family, this->font_size, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::PointF point_f(this->x, this->y - this->font_size);
    wstring wide_content(this->content.begin(), this->content.end());
    graphics.DrawString(wide_content.c_str(), -1, &font, point_f, &fill_pen);
}

Elip::Elip(Entity entity) : Shape(entity)
{
    this->cx = stof(entity.attributes["cx"]);
    this->cy = stof(entity.attributes["cy"]);
    this->rx = stof(entity.attributes["rx"]);
    this->ry = stof(entity.attributes["ry"]);
}
void Elip::render_ellipse(Gdiplus::Graphics &graphics)
{
    Gdiplus::Pen stroke_pen(Gdiplus::Color(this->stroke_color.alpha, this->stroke_color.red, this->stroke_color.green, this->stroke_color.blue), this->stroke_width);
    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(this->fill_color.alpha, this->fill_color.red, this->fill_color.green, this->fill_color.blue));
    Gdiplus::RectF shape(this->cx - this->rx, this->cy - this->ry, this->rx * 2, this->ry * 2);
    graphics.FillEllipse(&fill_pen, shape);
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

Plyline::Plyline(Entity entity) : Shape(entity)
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
void Plyline::render_polyline(Gdiplus::Graphics &graphics)
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

Plygon::Plygon(Entity entity) : Shape(entity)
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
void Plygon::render_polygon(Gdiplus::Graphics &graphics)
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