#include "color.h"

Color::Color()
{
    this->alpha = 255;
    this->red = 0;
    this->green = 0;
    this->blue = 0;
}
Color::Color(int red, int green, int blue)
{
    this->alpha = 255;
    this->red = red;
    this->green = green;
    this->blue = blue;
}
Color::Color(int alpha, int red, int green, int blue)
{
    this->alpha = alpha;
    this->red = red;
    this->green = green;
    this->blue = blue;
}
Color::Color(const string &opacity, const string &rgb)
{
    this->alpha = opacity.empty() ? 255 : static_cast<int>(255 * stof(opacity));
    if (!rgb.empty())
    {
        istringstream color_stream(rgb);
        string vessel;
        getline(color_stream, vessel, '(');
        color_stream >> this->red >> this->green >> this->blue;
    }
    else
    {
        this->red = 0;
        this->green = 0;
        this->blue = 0;
    }
}

unordered_map<string, Color> named_colors = {
    {"aliceblue", "rgb(240 248 255)"},
    {"antiquewhite", "rgb(250 235 215)"},
    {"aqua", "rgb(0 255 255)"},
    {"aquamarine", "rgb(127 255 212)"},
    {"azure", "rgb(240 255 255)"},
    {"beige", "rgb(245 245 220)"},
    {"bisque", "rgb(255 228 196)"},
    {"black", "rgb(0 0 0)"},
    {"blanchedalmond", "rgb(255 235 205)"},
    {"blue", "rgb(0 0 255)"},
    {"blueviolet", "rgb(138 43 226)"},
    {"brown", "rgb(165 42 42)"},
    {"burlywood", "rgb(222 184 135)"},
    {"cadetblue", "rgb(95 158 160)"},
    {"chartreuse", "rgb(127 255 0)"},
    {"chocolate", "rgb(210 105 30)"},
    {"coral", "rgb(255 127 80)"},
    {"cornflowerblue", "rgb(100 149 237)"},
    {"cornsilk", "rgb(255 248 220)"},
    {"crimson", "rgb(220 20 60)"},
    {"cyan", "rgb(0 255 255)"},
    {"darkblue", Color(0, 0, 139)},
    {"darkcyan", Color(0, 139, 139)},
    {"darkgoldenrod", Color(184, 134, 11)},
    {"darkgray", Color(169, 169, 169)},
    {"darkgreen", Color(0, 100, 0)},
    {"darkgrey", Color(169, 169, 169)},
    {"darkkhaki", Color(189, 183, 107)},
    {"darkmagenta", Color(139, 0, 139)},
    {"darkolivegreen", Color(85, 107, 47)},
    {"darkorange", Color(255, 140, 0)},
    {"darkorchid", Color(153, 50, 204)},
    {"darkred", Color(139, 0, 0)},
    {"darksalmon", Color(233, 150, 122)},
    {"darkseagreen", Color(143, 188, 143)},
    {"darkslateblue", Color(72, 61, 139)},
    {"darkslategray", Color(47, 79, 79)},
    {"darkslategrey", Color(47, 79, 79)},
    {"darkturquoise", Color(0, 206, 209)},
    {"darkviolet", Color(148, 0, 211)},
    {"deeppink", Color(255, 20, 147)},
    {"deepskyblue", Color(0, 191, 255)},
    {"dimgray", Color(105, 105, 105)},
    {"dimgrey", Color(105, 105, 105)},
    {"dodgerblue", Color(30, 144, 255)},
    {"firebrick", Color(178, 34, 34)},
    {"floralwhite", Color(255, 250, 240)},
    {"forestgreen", Color(34, 139, 34)},
    {"fuchsia", Color(255, 0, 255)},
    {"gainsboro", Color(220, 220, 220)},
    {"ghostwhite", Color(248, 248, 255)},
    {"gold", Color(255, 215, 0)},
    {"goldenrod", Color(218, 165, 32)},
    {"gray", Color(128, 128, 128)},
    {"grey", Color(128, 128, 128)},
    {"green", Color(0, 128, 0)},
    {"greenyellow", Color(173, 255, 47)},
    {"honeydew", Color(240, 255, 240)},
    {"hotpink", Color(255, 105, 180)},
    {"indianred", Color(205, 92, 92)},
    {"indigo", Color(75, 0, 130)},
    {"ivory", Color(255, 255, 240)},
    {"khaki", Color(240, 230, 140)},
    {"lavender", Color(230, 230, 250)},
    {"lavenderblush", Color(255, 240, 245)},
    {"lawngreen", Color(124, 252, 0)},
    {"lemonchiffon", Color(255, 250, 205)},
    {"lightblue", Color(173, 216, 230)},
    {"lightcoral", Color(240, 128, 128)},
    {"lightcyan", Color(224, 255, 255)},
    {"lightgoldenrodyellow", Color(250, 250, 210)},
    {"lightgray", Color(211, 211, 211)},
    {"lightgreen", Color(144, 238, 144)},
    {"lightgrey", Color(211, 211, 211)},
    {"lightpink", Color(255, 182, 193)},
    {"lightsalmon", Color(255, 160, 122)},
    {"lightseagreen", Color(32, 178, 170)},
    {"lightskyblue", Color(135, 206, 250)},
    {"lightslategray", Color(119, 136, 153)},
    {"lightslategrey", Color(119, 136, 153)},
    {"lightsteelblue", Color(176, 196, 222)},
    {"lightyellow", Color(255, 255, 224)},
    {"lime", Color(0, 255, 0)},
    {"limegreen", Color(50, 205, 50)},
    {"linen", Color(250, 240, 230)},
    {"magenta", Color(255, 0, 255)},
    {"maroon", Color(128, 0, 0)},
    {"mediumaquamarine", Color(102, 205, 170)},
    {"mediumblue", Color(0, 0, 205)},
    {"mediumorchid", Color(186, 85, 211)},
    {"mediumpurple", Color(147, 112, 219)},
    {"mediumseagreen", Color(60, 179, 113)},
    {"mediumslateblue", Color(123, 104, 238)},
    {"mediumspringgreen", Color(0, 250, 154)},
    {"mediumturquoise", Color(72, 209, 204)},
    {"mediumvioletred", Color(199, 21, 133)},
    {"midnightblue", Color(25, 25, 112)},
    {"mintcream", Color(245, 255, 250)},
    {"mistyrose", Color(255, 228, 225)},
    {"moccasin", Color(255, 228, 181)},
    {"navajowhite", Color(255, 222, 173)},
    {"navy", Color(0, 0, 128)},
    {"oldlace", Color(253, 245, 230)},
    {"olive", Color(128, 128, 0)},
    {"olivedrab", Color(107, 142, 35)},
    {"orange", Color(255, 165, 0)},
    {"orangered", Color(255, 69, 0)},
    {"orchid", Color(218, 112, 214)},
    {"palegoldenrod", Color(238, 232, 170)},
    {"palegreen", Color(152, 251, 152)},
    {"paleturquoise", Color(175, 238, 238)},
    {"palevioletred", Color(219, 112, 147)},
    {"papayawhip", Color(255, 239, 213)},
    {"peachpuff", Color(255, 218, 185)},
    {"peru", Color(205, 133, 63)},
    {"pink", Color(255, 192, 203)},
    {"plum", Color(221, 160, 221)},
    {"powderblue", Color(176, 224, 230)},
    {"purple", Color(128, 0, 128)},
    {"red", Color(255, 0, 0)},
    {"rosybrown", Color(188, 143, 143)},
    {"royalblue", Color(65, 105, 225)},
    {"saddlebrown", Color(139, 69, 19)},
    {"salmon", Color(250, 128, 114)},
    {"sandybrown", Color(244, 164, 96)},
    {"seagreen", Color(46, 139, 87)},
    {"seashell", Color(255, 245, 238)},
    {"sienna", Color(160, 82, 45)},
    {"silver", Color(192, 192, 192)},
    {"skyblue", Color(135, 206, 235)},
    {"slateblue", Color(106, 90, 205)},
    {"slategray", Color(112, 128, 144)},
    {"slategrey", Color(112, 128, 144)},
    {"snow", Color(255, 250, 250)},
    {"springgreen", Color(0, 255, 127)},
    {"steelblue", Color(70, 130, 180)},
    {"tan", Color(210, 180, 140)},
    {"teal", Color(0, 128, 128)},
    {"thistle", Color(216, 191, 216)},
    {"tomato", Color(255, 99, 71)},
    {"turquoise", Color(64, 224, 208)},
    {"violet", Color(238, 130, 238)},
    {"wheat", Color(245, 222, 179)},
    {"white", Color(255, 255, 255)},
    {"whitesmoke", Color(245, 245, 245)},
    {"yellow", Color(255, 255, 0)},
    {"yellowgreen", Color(154, 205, 50)},
};

void Color::hex_to_rgb(const string &hex_color)
{
    string vessel = (hex_color[0] == '#') ? hex_color.substr(1) : hex_color;
    if (vessel.length() < 6)
        vessel = string(2, vessel[0]) + string(2, vessel[1]) + string(2, vessel[2]);

    istringstream(vessel.substr(0, 2)) >> hex >> this->red;
    istringstream(vessel.substr(2, 2)) >> hex >> this->green;
    istringstream(vessel.substr(4, 2)) >> hex >> this->blue;
}
Color::Color(const string &named_color)
{
    if (named_color[0] == '#')
        this->hex_to_rgb(named_color);
    else
    {
        auto vessel = named_colors.find(named_color);
        *this = vessel != named_colors.end() ? vessel->second : Color(255, 0, 0, 0);
    }
}

Color::Color(const string &type, unordered_map<string, string> &attributes)
{
    if (attributes.find(type) != attributes.end())
    {
        if (attributes[type] == "none" || attributes[type] == "transparent")
            *this = Color(0, 0, 0, 0);
        else if (attributes[type].find("rgb") != string::npos)
            *this = attributes.find(type + "-opacity") != attributes.end() ? Color(attributes[type + "-opacity"], attributes[type]) : Color("", attributes[type]);
        else
        {
            *this = Color(attributes[type]);
            this->alpha = attributes.find(type + "-opacity") != attributes.end() ? static_cast<int>(255 * stof(attributes[type + "-opacity"])) : 255;
        }
    }
    else
    {
        *this = Color(0, 0, 0);
        this->alpha = attributes.find(type + "-opacity") != attributes.end() ? static_cast<int>(255 * stof(attributes[type + "-opacity"])) : 255;
    }
}
