#include "shape.h"
#include <memory>
using namespace Shapes;

// Shape Class
Shape::Shape()
{
    this->stroke_width = 0;
    this->stroke_color = Color(255, 0, 0, 0);
    this->fill_color = Color(255, 0, 0, 0);
}
Shape::Shape(Entity entity)
{
    if (entity.attributes.find("stroke") != entity.attributes.end())
        this->stroke_width = entity.attributes.find("stroke-width") != entity.attributes.end() ? stof(entity.attributes["stroke-width"]) : 1;
    else
        this->stroke_width = 0;
    this->fill_rule = (entity.attributes.find("fill-rule") != entity.attributes.end()) ? entity.attributes["fill-rule"] : "nonzero";

    this->stroke_color = Color("stroke", entity.attributes);
    if (this->stroke_color == Color(-1, -1, -1))
        this->stroke_gradient_id = entity.attributes["stroke"];

    this->fill_color = Color("fill", entity.attributes);
    if (this->fill_color == Color(-1, -1, -1))
        this->fill_gradient_id = entity.attributes["fill"];

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
        {
            if (transform.values.size() > 1)
                graphics.TranslateTransform(transform.values[0], transform.values[1]);
            else
                graphics.TranslateTransform(transform.values[0], 0);
        }
        else if (transform.type == "rotate")
            graphics.RotateTransform(transform.values[0]);
        else if (transform.type == "scale")
        {
            if (transform.values.size() > 1)
                graphics.ScaleTransform(transform.values[0], transform.values[1]);
            else
                graphics.ScaleTransform(transform.values[0], transform.values[0]);
        }
        else if (transform.type == "matrix")
        {
            if (transform.values.size() > 5)
            {
                Gdiplus::Matrix matrix(transform.values[0], transform.values[1], transform.values[2], transform.values[3], transform.values[4], transform.values[5]);
                graphics.MultiplyTransform(&matrix);
            }
        }
    }
}

void Shape::apply_gradient_transform(Gdiplus::LinearGradientBrush &brush, vector<Transform> gradient_transform)
{
    for (const auto &transform : gradient_transform)
    {
        if (transform.type == "translate")
        {
            if (transform.values.size() > 1)
                brush.TranslateTransform(transform.values[0], transform.values[1]), Gdiplus::MatrixOrderAppend;
            else
                brush.TranslateTransform(transform.values[0], 0, Gdiplus::MatrixOrderAppend);
        }
        else if (transform.type == "rotate")
            brush.RotateTranform(transform.values[0], Gdiplus::MatrixOrderAppend);
        else if (transform.type == "scale")
        {
            if (transform.values.size() > 1)
                brush.ScaleTransform(transform.values[0], transform.values[1], Gdiplus::MatrixOrderAppend);
            else
                brush.ScaleTransform(transform.values[0], transform.values[0], Gdiplus::MatrixOrderAppend);
        }
        else if (transform.type == "matrix")
        {
            if (transform.values.size() > 5)
            {
                Gdiplus::Matrix matrix(transform.values[0], transform.values[1], transform.values[2], transform.values[3], transform.values[4], transform.values[5]);
                brush.MultiplyTransform(&matrix, Gdiplus::MatrixOrderAppend);
            }
        }
    }
}

void Shape::apply_gradient_transform(Gdiplus::PathGradientBrush &brush, vector<Transform> gradient_transform)
{
    for (const auto &transform : gradient_transform)
    {
        if (transform.type == "translate")
        {
            if (transform.values.size() > 1)
                brush.TranslateTransform(transform.values[0], transform.values[1], Gdiplus::MatrixOrderAppend);
            else
                brush.TranslateTransform(transform.values[0], 0, Gdiplus::MatrixOrderAppend);
        }
        else if (transform.type == "rotate")
            brush.RotateTransform(transform.values[0], Gdiplus::MatrixOrderAppend);
        else if (transform.type == "scale")
        {
            if (transform.values.size() > 1)
                brush.ScaleTransform(transform.values[0], transform.values[1], Gdiplus::MatrixOrderAppend);
            else
                brush.ScaleTransform(transform.values[0], transform.values[0], Gdiplus::MatrixOrderAppend);
        }
        else if (transform.type == "matrix")
        {
            if (transform.values.size() > 5)
            {
                Gdiplus::Matrix matrix(transform.values[0], transform.values[1], transform.values[2], transform.values[3], transform.values[4], transform.values[5]);
                brush.MultiplyTransform(&matrix, Gdiplus::MatrixOrderAppend);
            }
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

    if (!font_family->IsAvailable())
        font_family.reset(new Gdiplus::FontFamily(L"times new roman"));

    Gdiplus::FontStyle font_style = this->font_style_map[this->font_style];

    Gdiplus::PointF origin(this->x + this->dx, this->y + this->dy - 0.9 * this->font_size);
    origin.X += offset_map[this->text_anchor];

    Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericDefault());
    format.SetAlignment(this->text_anchor_map[this->text_anchor]);

    Gdiplus::GraphicsPath path;
    path.SetFillMode(this->fill_rule_map[this->fill_rule]);
    path.AddString(wide_content.c_str(), -1, font_family.get(), font_style, this->font_size, origin, &format);

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

    Gdiplus::GraphicsPath path;
    path.SetFillMode(this->fill_rule_map[this->fill_rule]);
    for (size_t i = 0; i < points.size() - 1; i += 1)
        path.AddLine(points[i].first, points[i].second, points[i + 1].first, points[i + 1].second);
    Gdiplus::SolidBrush fill_pen(Gdiplus::Color(fill_color.alpha, fill_color.red, fill_color.green, fill_color.blue));
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
    graphics.FillPolygon(&fill_pen, gdiplus_points.data(), gdiplus_points.size(), this->fill_rule_map[this->fill_rule]);

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
    string vessel;
    istringstream stream(entity.attributes["d"]);

    while (stream >> vessel)
    {
        char command = vessel[0];
        if (isalpha(command))
        {
            PathSegment node;
            node.type = command;
            while (stream >> vessel)
            {
                if (isalpha(vessel[0]))
                {
                    stream.seekg(-vessel.size(), ios::cur);
                    break;
                }
                node.points.push_back(stof(vessel));
            }
            this->separated_data.push_back(node);
        }
    }
}
void Path::render(Gdiplus::Graphics &graphics)
{
    Gdiplus::GraphicsState state = graphics.Save();
    this->apply_transform(graphics);

    Gdiplus::GraphicsPath path;
    path.SetFillMode(this->fill_rule_map[this->fill_rule]);

    Gdiplus::PointF open_point;
    Gdiplus::PointF current_point;
    Gdiplus::PointF previous_control_point;
    Gdiplus::PointF previous_quad_point;

    for (size_t k = 0; k < this->separated_data.size(); k += 1)
    {
        if (separated_data[k].type == 'M')
        {
            if (separated_data[k].points.size() > 1)
            {
                path.StartFigure();
                open_point = Gdiplus::PointF(separated_data[k].points[0], separated_data[k].points[1]);
                current_point = open_point;
                for (size_t i = 2; i < separated_data[k].points.size(); i += 2)
                {
                    if (i + 1 < separated_data[k].points.size())
                    {
                        Gdiplus::PointF end_point(separated_data[k].points[i], separated_data[k].points[i + 1]);
                        path.AddLine(current_point, end_point);
                        current_point = end_point;
                    }
                }
            }
        }

        else if (separated_data[k].type == 'L')
        {
            for (size_t i = 0; i < separated_data[k].points.size(); i += 2)
            {
                if (i + 1 < separated_data[k].points.size())
                {
                    Gdiplus::PointF end_point(separated_data[k].points[i], separated_data[k].points[i + 1]);
                    path.AddLine(current_point, end_point);
                    current_point = end_point;
                }
            }
        }

        else if (separated_data[k].type == 'H')
        {
            for (size_t i = 0; i < separated_data[k].points.size(); i += 1)
            {
                Gdiplus::PointF end_point(separated_data[k].points[i], current_point.Y);
                path.AddLine(current_point, end_point);
                current_point = end_point;
            }
        }

        else if (separated_data[k].type == 'V')
        {
            for (size_t i = 0; i < separated_data[k].points.size(); i += 1)
            {
                Gdiplus::PointF end_point(current_point.X, separated_data[k].points[i]);
                path.AddLine(current_point, end_point);
                current_point = end_point;
            }
        }

        else if (separated_data[k].type == 'C')
        {
            for (size_t i = 0; i < separated_data[k].points.size(); i += 6)
            {
                if (i + 5 < separated_data[k].points.size())
                {
                    Gdiplus::PointF control_point_first(separated_data[k].points[i], separated_data[k].points[i + 1]);
                    Gdiplus::PointF control_point_second(separated_data[k].points[i + 2], separated_data[k].points[i + 3]);
                    Gdiplus::PointF end_point(separated_data[k].points[i + 4], separated_data[k].points[i + 5]);
                    path.AddBezier(current_point, control_point_first, control_point_second, end_point);
                    previous_control_point = control_point_second;
                    current_point = end_point;
                }
            }
        }

        else if (separated_data[k].type == 'S')
        {
            Gdiplus::PointF control_point_first = current_point;
            if (k > 1 && (tolower(separated_data[k - 1].type) == 'c' || tolower(separated_data[k - 1].type) == 's'))
                control_point_first = Gdiplus::PointF(2 * current_point.X - previous_control_point.X, 2 * current_point.Y - previous_control_point.Y);

            for (size_t i = 0; i < separated_data[k].points.size(); i += 4)
            {
                if (i + 3 < separated_data[k].points.size())
                {
                    Gdiplus::PointF control_point_second(separated_data[k].points[i], separated_data[k].points[i + 1]);
                    Gdiplus::PointF end_point(separated_data[k].points[i + 2], separated_data[k].points[i + 3]);
                    path.AddBezier(current_point, control_point_first, control_point_second, end_point);
                    previous_control_point = control_point_second;
                    current_point = end_point;
                }
            }
        }

        else if (separated_data[k].type == 'A')
        {
            for (size_t j = 0; j < separated_data[k].points.size(); j += 7)
            {
                if (j + 6 < separated_data[k].points.size())
                {
                    vector<double> previous_point = {current_point.X, current_point.Y};
                    vector<vector<vector<double>>> curves = arc_to_cubic_beziers(previous_point, separated_data[k].points[j + 0], separated_data[k].points[j + 1], separated_data[k].points[j + 2], separated_data[k].points[j + 3], separated_data[k].points[j + 4], separated_data[k].points[j + 5], separated_data[k].points[j + 6]);
                    for (const auto &curve : curves)
                    {
                        for (size_t i = 0; i < curve.size(); i += 3)
                        {
                            if (i + 2 < curve.size())
                            {
                                Gdiplus::PointF control_point1(curve[i][0], curve[i][1]);
                                Gdiplus::PointF control_point2(curve[i + 1][0], curve[i + 1][1]);
                                Gdiplus::PointF end_point(curve[i + 2][0], curve[i + 2][1]);
                                path.AddBezier(current_point, control_point1, control_point2, end_point);
                                current_point = end_point;
                            }
                        }
                    }
                }
            }
        }

        else if (separated_data[k].type == 'Q')
        {
            for (size_t i = 0; i < separated_data[k].points.size(); i += 4)
            {
                if (i + 3 < separated_data[k].points.size())
                {
                    Gdiplus::PointF control_point_first((current_point.X + 2 * separated_data[k].points[i]) / 3, (current_point.Y + 2 * separated_data[k].points[i + 1]) / 3);
                    Gdiplus::PointF control_point_second((separated_data[k].points[i + 2] + 2 * separated_data[k].points[i]) / 3, (separated_data[k].points[i + 3] + 2 * separated_data[k].points[i + 1]) / 3);
                    Gdiplus::PointF end_point(separated_data[k].points[i + 2], separated_data[k].points[i + 3]);
                    path.AddBezier(current_point, control_point_first, control_point_second, end_point);
                    previous_quad_point = Gdiplus::PointF(separated_data[k].points[i], separated_data[k].points[i + 1]);
                    current_point = end_point;
                }
            }
        }

        else if (separated_data[k].type == 'T')
        {
            double quad_X = current_point.X;
            double quad_Y = current_point.Y;
            if (k > 1 && (tolower(separated_data[k - 1].type) == 'q' || tolower(separated_data[k - 1].type) == 't'))
            {
                quad_X = 2 * current_point.X - previous_quad_point.X;
                quad_Y = 2 * current_point.Y - previous_quad_point.Y;
            }
            for (size_t i = 0; i < separated_data[k].points.size(); i += 2)
            {
                if (i + 1 < separated_data[k].points.size())
                {
                    Gdiplus::PointF control_point_first((current_point.X + 2 * quad_X) / 3, (current_point.Y + 2 * quad_Y) / 3);
                    Gdiplus::PointF control_point_second((separated_data[k].points[i] + 2 * quad_X) / 3, (separated_data[k].points[i + 1] + 2 * quad_Y) / 3);
                    Gdiplus::PointF end_point(separated_data[k].points[i], separated_data[k].points[i + 1]);
                    path.AddBezier(current_point, control_point_first, control_point_second, end_point);
                    previous_quad_point = Gdiplus::PointF(quad_X, quad_Y);
                    current_point = end_point;
                }
            }
        }

        else if (separated_data[k].type == 'm')
        {
            if (separated_data[k].points.size() > 1)
            {
                path.StartFigure();
                open_point = Gdiplus::PointF(current_point.X + separated_data[k].points[0], current_point.Y + separated_data[k].points[1]);
                current_point = open_point;
                for (size_t i = 2; i < separated_data[k].points.size(); i += 2)
                {
                    if (i + 1 < separated_data[k].points.size())
                    {
                        Gdiplus::PointF end_point(current_point.X + separated_data[k].points[i], current_point.Y + separated_data[k].points[i + 1]);
                        path.AddLine(current_point, end_point);
                        current_point = end_point;
                    }
                }
            }
        }

        else if (separated_data[k].type == 'l')
        {
            for (size_t i = 0; i < separated_data[k].points.size(); i += 2)
            {
                if (i + 1 < separated_data[k].points.size())
                {
                    Gdiplus::PointF end_point(current_point.X + separated_data[k].points[i], current_point.Y + separated_data[k].points[i + 1]);
                    path.AddLine(current_point, end_point);
                    current_point = end_point;
                }
            }
        }

        else if (separated_data[k].type == 'h')
        {
            for (size_t i = 0; i < separated_data[k].points.size(); i += 1)
            {
                Gdiplus::PointF end_point(current_point.X + separated_data[k].points[i], current_point.Y);
                path.AddLine(current_point, end_point);
                current_point = end_point;
            }
        }

        else if (separated_data[k].type == 'v')
        {
            for (size_t i = 0; i < separated_data[k].points.size(); i += 1)
            {
                Gdiplus::PointF end_point(current_point.X, current_point.Y + separated_data[k].points[i]);
                path.AddLine(current_point, end_point);
                current_point = end_point;
            }
        }

        else if (separated_data[k].type == 'c')
        {
            for (size_t i = 0; i < separated_data[k].points.size(); i += 6)
            {
                if (i + 5 < separated_data[k].points.size())
                {
                    Gdiplus::PointF control_point_first(current_point.X + separated_data[k].points[i], current_point.Y + separated_data[k].points[i + 1]);
                    Gdiplus::PointF control_point_second(current_point.X + separated_data[k].points[i + 2], current_point.Y + separated_data[k].points[i + 3]);
                    Gdiplus::PointF end_point(current_point.X + separated_data[k].points[i + 4], current_point.Y + separated_data[k].points[i + 5]);
                    path.AddBezier(current_point, control_point_first, control_point_second, end_point);
                    previous_control_point = control_point_second;
                    current_point = end_point;
                }
            }
        }

        else if (separated_data[k].type == 's')
        {
            Gdiplus::PointF control_point_first = current_point;
            if (k > 1 && (tolower(separated_data[k - 1].type) == 'c' || tolower(separated_data[k - 1].type) == 's'))
                control_point_first = Gdiplus::PointF(2 * current_point.X - previous_control_point.X, 2 * current_point.Y - previous_control_point.Y);

            for (size_t i = 0; i < separated_data[k].points.size(); i += 4)
            {
                if (i + 3 < separated_data[k].points.size())
                {
                    Gdiplus::PointF control_point_second(current_point.X + separated_data[k].points[i], current_point.Y + separated_data[k].points[i + 1]);
                    Gdiplus::PointF end_point(current_point.X + separated_data[k].points[i + 2], current_point.Y + separated_data[k].points[i + 3]);
                    path.AddBezier(current_point, control_point_first, control_point_second, end_point);
                    previous_control_point = control_point_second;
                    current_point = end_point;
                }
            }
        }

        else if (separated_data[k].type == 'a')
        {
            for (size_t j = 0; j < separated_data[k].points.size(); j += 7)
            {
                if (j + 6 < separated_data[k].points.size())
                {
                    vector<double> previous_point = {current_point.X, current_point.Y};
                    vector<vector<vector<double>>> curves = arc_to_cubic_beziers(previous_point, separated_data[k].points[j + 0], separated_data[k].points[j + 1], separated_data[k].points[j + 2], separated_data[k].points[j + 3], separated_data[k].points[j + 4], separated_data[k].points[j + 5] + previous_point[0], separated_data[k].points[j + 6] + previous_point[1]);
                    for (const auto &curve : curves)
                    {
                        for (size_t i = 0; i < curve.size(); i += 3)
                        {
                            if (i + 2 < curve.size())
                            {
                                Gdiplus::PointF control_point1(curve[i][0], curve[i][1]);
                                Gdiplus::PointF control_point2(curve[i + 1][0], curve[i + 1][1]);
                                Gdiplus::PointF end_point(curve[i + 2][0], curve[i + 2][1]);
                                path.AddBezier(current_point, control_point1, control_point2, end_point);
                                current_point = end_point;
                            }
                        }
                    }
                }
            }
        }

        else if (separated_data[k].type == 'q')
        {
            for (size_t i = 0; i < separated_data[k].points.size(); i += 4)
            {
                if (i + 3 < separated_data[k].points.size())
                {
                    double x1 = separated_data[k].points[i] + current_point.X;
                    double y1 = separated_data[k].points[i + 1] + current_point.Y;
                    double x2 = separated_data[k].points[i + 2] + current_point.X;
                    double y2 = separated_data[k].points[i + 3] + current_point.Y;
                    Gdiplus::PointF control_point_first((current_point.X + 2 * x1) / 3, (current_point.Y + 2 * y1) / 3);
                    Gdiplus::PointF control_point_second((x2 + 2 * x1) / 3, (y2 + 2 * y1) / 3);
                    Gdiplus::PointF end_point(x2, y2);
                    path.AddBezier(current_point, control_point_first, control_point_second, end_point);
                    previous_quad_point = Gdiplus::PointF(x1, y1);
                    current_point = end_point;
                }
            }
        }

        else if (separated_data[k].type == 't')
        {
            double quad_X = current_point.X;
            double quad_Y = current_point.Y;
            if (k > 1 && (tolower(separated_data[k - 1].type) == 'q' || tolower(separated_data[k - 1].type) == 't'))
            {
                quad_X = 2 * current_point.X - previous_quad_point.X;
                quad_Y = 2 * current_point.Y - previous_quad_point.Y;
            }
            for (size_t i = 0; i < separated_data[k].points.size(); i += 2)
            {
                if (i + 1 < separated_data[k].points.size())
                {
                    double x1 = separated_data[k].points[i] + current_point.X;
                    double y1 = separated_data[k].points[i + 1] + current_point.Y;
                    Gdiplus::PointF control_point_first((current_point.X + 2 * quad_X) / 3, (current_point.Y + 2 * quad_Y) / 3);
                    Gdiplus::PointF control_point_second((x1 + 2 * quad_X) / 3, (y1 + 2 * quad_Y) / 3);
                    Gdiplus::PointF end_point(x1, y1);
                    path.AddBezier(current_point, control_point_first, control_point_second, end_point);
                    previous_quad_point = Gdiplus::PointF(quad_X, quad_Y);
                    current_point = end_point;
                }
            }
        }

        else if (separated_data[k].type == 'Z' || separated_data[k].type == 'z')
        {
            path.CloseFigure();
            current_point = open_point;
        }
    }
    if (this->fill_gradient_id.empty())
    {
        Gdiplus::SolidBrush fill_pen(Gdiplus::Color(fill_color.alpha, fill_color.red, fill_color.green, fill_color.blue));
        graphics.FillPath(&fill_pen, &path);
    }
    else
    {
        GradientColor gradient_color = gradient_system.find_gradient_by_id(this->fill_gradient_id);
        vector<Gdiplus::Color> colors;
        if (!gradient_color.stop_colors.empty())
        {
            colors.push_back(Gdiplus::Color(gradient_color.stop_colors.front().alpha, gradient_color.stop_colors.front().red, gradient_color.stop_colors.front().green, gradient_color.stop_colors.front().blue));
            for (const Color &color : gradient_color.stop_colors)
                colors.push_back(Gdiplus::Color(color.alpha, color.red, color.green, color.blue));
            colors.push_back(Gdiplus::Color(gradient_color.stop_colors.back().alpha, gradient_color.stop_colors.back().red, gradient_color.stop_colors.back().green, gradient_color.stop_colors.front().blue));
        }
        else
            colors.push_back(Gdiplus::Color(0, 0, 0, 0));

        if (gradient_color.type == "lineargradient")
        {
            Gdiplus::PointF start_point(gradient_color.attributes.find("x1") != gradient_color.attributes.end() ? stof(gradient_color.attributes["x1"]) : 0, gradient_color.attributes.find("y1") != gradient_color.attributes.end() ? stof(gradient_color.attributes["y1"]) : 0);
            Gdiplus::PointF end_point(gradient_color.attributes.find("x2") != gradient_color.attributes.end() ? stof(gradient_color.attributes["x2"]) : 0, gradient_color.attributes.find("y2") != gradient_color.attributes.end() ? stof(gradient_color.attributes["y2"]) : 0);
            Gdiplus::LinearGradientBrush linear_gradient_brush(start_point, end_point, colors.front(), colors.back());
            this->apply_gradient_transform(linear_gradient_brush, gradient_color.gradient_transforms);
            linear_gradient_brush.SetWrapMode(Gdiplus::WrapModeTileFlipXY);
            linear_gradient_brush.SetInterpolationColors(&colors[0], &gradient_color.offset[0], gradient_color.offset.size());
            graphics.FillPath(&linear_gradient_brush, &path);
        }
        else if (gradient_color.type == "radialgradient")
        {
            float cx = gradient_color.attributes.find("cx") != gradient_color.attributes.end() ? stof(gradient_color.attributes["cx"]) : 0;
            float cy = gradient_color.attributes.find("cy") != gradient_color.attributes.end() ? stof(gradient_color.attributes["cy"]) : 0;
            float r = gradient_color.attributes.find("r") != gradient_color.attributes.end() ? stof(gradient_color.attributes["r"]) : 0;
            Gdiplus::PointF start_point(cx, cy);
            Gdiplus::PointF end_point(cx + r, cy + r);
            Gdiplus::LinearGradientBrush linear_gradient_brush(start_point, end_point, colors.front(), colors.back());
            this->apply_gradient_transform(linear_gradient_brush, gradient_color.gradient_transforms);
            linear_gradient_brush.SetWrapMode(Gdiplus::WrapModeTileFlipXY);
            graphics.FillPath(&linear_gradient_brush, &path);
        }
    }

    if (stroke_width != 0)
    {
        Gdiplus::Pen stroke_pen(Gdiplus::Color(stroke_color.alpha, stroke_color.red, stroke_color.green, stroke_color.blue), stroke_width);
        graphics.DrawPath(&stroke_pen, &path);
    }
    graphics.Restore(state);
}