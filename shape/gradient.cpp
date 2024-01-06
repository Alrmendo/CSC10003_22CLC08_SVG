#include "gradient.h"

GradientSystem::GradientSystem() {}
GradientSystem::GradientSystem(const Entity &defs_root)
{
    this->main_data = defs_root;
}

string GradientSystem::get_id_from_string(const string &input)
{
    size_t vessel = input.find("#");
    if (vessel != string::npos)
        return input.substr(vessel + 1, input.size() - vessel - 1 - (input.back() == ')'));
    return input;
}
Entity GradientSystem::find_entity_by_id(const string &id)
{
    string vessel = get_id_from_string(id);
    for (Entity &child : this->main_data.children)
    {
        if (child.attributes.find("id") != child.attributes.end() && child.attributes["id"] == vessel)
            return child;
    }
    return Entity();
}
GradientColor GradientSystem::find_gradient_by_id(const string &id)
{
    Entity entity = this->find_entity_by_id(id);
    if (entity.attributes.find("xlink:href") != entity.attributes.end())
    {
        Entity link = this->find_entity_by_id(entity.attributes["xlink:href"]);
        for (auto &child : link.children)
            entity.children.push_back(child);
        for (const auto &attr : link.attributes)
        {
            if (entity.attributes.find(attr.first) == entity.attributes.end())
                entity.attributes[attr.first] = attr.second;
        }
    }
    GradientColor vessel;
    vessel.type = entity.type;
    vessel.attributes = entity.attributes;

    vessel.offset.push_back(0.0f);
    for (Entity &child : entity.children)
    {
        if (child.type == "stop")
        {
            string stop_color = child.attributes.find("stop-color") != child.attributes.end() ? child.attributes["stop-color"] : "#000";
            string stop_opacity = child.attributes.find("stop-opacity") != child.attributes.end() ? child.attributes["stop-opacity"] : "1";
            float offset = child.attributes.find("offset") != child.attributes.end() ? stof(child.attributes["offset"]) : 0;
            vessel.offset.push_back(offset);
            vessel.stop_colors.push_back(Color(stop_opacity, stop_color));
        }
    }
    vessel.offset.push_back(1.0f);

    if (entity.attributes.find("gradienttransform") != entity.attributes.end())
    {
        istringstream element_stream(entity.attributes["gradienttransform"]);
        string single_transform;
        while (getline(element_stream, single_transform, ')'))
        {
            Transform transform_vessel;
            string values;
            float value;

            istringstream transform_stream(single_transform);
            getline(transform_stream, transform_vessel.type, '(');
            getline(transform_stream, values);
            istringstream values_stream(values);
            while (values_stream >> value)
                transform_vessel.values.push_back(value);
            transform_vessel.type = format_text(transform_vessel.type);
            vessel.gradient_transforms.push_back(transform_vessel);
        }
    }
    return vessel;
}

GradientSystem gradient_system;
