#ifndef __GRADIENT__
#define __GRADIENT__
#include "../reader/read_file.h"
#include "color.h"
#include <map>
struct GradientColor
{
    string type;
    unordered_map<string, string> attributes;
    vector<float> offset;
    vector<Color> stop_colors;
    vector<Transform> gradient_transforms;
};

class GradientSystem
{
private:
    Entity main_data;

public:
    GradientSystem();
    GradientSystem(const Entity &);
    Entity find_entity_by_id(const string &);
    GradientColor find_gradient_by_id(const string &);
    string get_id_from_string(const string &);
};
extern GradientSystem gradient_system;
#endif
