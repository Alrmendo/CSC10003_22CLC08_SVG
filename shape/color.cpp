#include "color.h"
Color::Color()
{
    this->alpha = 255;
    this->red = 0;
    this->green = 0;
    this->blue = 0;
}

Color::Color(int alpha, int red, int green, int blue)
{
    this->alpha = alpha;
    this->red = red;
    this->green = green;
    this->blue = blue;
}

Color::Color(const string &named_color)
{
    if (named_color == "black")
        Color(255, 0, 0, 0);
    else if (named_color == "white")
        Color(255, 255, 255, 255);
    else if (named_color == "red")
        Color(255, 255, 0, 0);
    else if (named_color == "green")
        Color(255, 0, 255, 0);
    else if (named_color == "blue")
        Color(255, 0, 0, 255);
}
