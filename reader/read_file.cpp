#include "read_file.h"

void SvgParser::traverse_node(const xml_node<> *node, Entity &entity, int depth)
{
    entity.type = to_lower(node->name());

    for (const xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
    {
        string attr_value = replace_comma_with_space(attr->value());
        if (to_lower(attr->name()) == "d")
            attr_value = add_space_between_char_and_number(attr_value);
        else
            attr_value = to_lower(attr_value);
        if (to_lower(attr->name()) == "style")
        {
            istringstream style_stream(attr_value);
            string vessel;
            while (getline(style_stream, vessel, ';'))
            {
                size_t colon_position = vessel.find(':');
                if (colon_position != string::npos)
                {
                    string property = vessel.substr(0, colon_position);
                    string value = vessel.substr(colon_position + 1);
                    entity.attributes[format_text(property)] = format_text(value);
                }
            }
        }
        entity.attributes[to_lower(attr->name())] = format_text(attr_value);
    }

    entity.text_content = format_text(node->value());
    entity.depth = depth;

    for (const xml_node<> *child = node->first_node(); child; child = child->next_sibling())
    {
        if (child->name() && child->name_size() > 0)
        {
            Entity child_entity;
            traverse_node(child, child_entity, depth + 1);
            entity.children.push_back(child_entity);
        }
    }
}

void SvgParser::parse_file()
{
    ifstream file(filename);
    if (!file.is_open())
        cout << "File not found\n";
    else
    {
        vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        buffer.push_back('\0');

        xml_document<> document;
        document.parse<0>(&buffer[0]);

        const xml_node<> *root_node = document.first_node();
        traverse_node(root_node, data);

        document.clear();
    }
    file.close();
}

void SvgParser::traverse_svg_data_iterative() const
{
    vector<Entity> stack;
    stack.push_back(this->data);

    while (!stack.empty())
    {
        Entity entity = stack.back();
        stack.pop_back();

        cout << entity.depth << " ";
        cout << string(4 * entity.depth, ' ') << "type: " << entity.type << endl;

        for (const auto &attr : entity.attributes)
            cout << string(4 * (entity.depth + 1), ' ') << attr.first << " = " << attr.second << endl;

        if (!entity.text_content.empty())
            cout << string(4 * (entity.depth + 1), ' ') << "content: " << entity.text_content << endl;

        if (entity.type == "g")
        {
            for (auto &child : entity.children)
            {
                for (const auto &attr : entity.attributes)
                {
                    if (child.attributes.find(attr.first) == child.attributes.end())
                        child.attributes[attr.first] = attr.second;
                    else
                    {
                        if (attr.first == "transform")
                            child.attributes["transform"] = attr.second + " " + child.attributes["transform"];
                    }
                }
            }
        }

        for (auto iter = entity.children.rbegin(); iter != entity.children.rend(); ++iter)
            stack.push_back(*iter);
    }
}

SvgParser::SvgParser(const string &filename)
{
    this->filename = filename;
    this->parse_file(filename);
}

Entity SvgParser::get_data()
{
    return this->data;
}

string to_lower(const string &input)
{
    string vessel;
    for (size_t i = 0; i < input.size(); i += 1)
        vessel.push_back(tolower(input[i]));
    return vessel;
}

string replace_comma_with_space(const string &input)
{
    string vessel;
    for (size_t i = 0; i < input.size(); i += 1)
    {
        if (input[i] == ',' || isspace(input[i]))
            vessel.push_back(' ');
        else
            vessel.push_back(input[i]);
    }
    return vessel;
}

string add_space_between_char_and_number(const string &input)
{
    string vessel;
    for (size_t i = 0; i < input.size() - 1; i += 1)
    {
        vessel.push_back(input[i]);
        if ((isalpha(input[i])) || (isdigit(input[i]) && input[i + 1] != '.' && !isdigit(input[i + 1])))
            vessel.push_back(' ');
    }
    vessel.push_back(input.back());
    return vessel;
}

string format_text(const string &input)
{
    string vessel;
    size_t first_non_space = input.find_first_not_of(" \t\n\r\f\v");
    size_t last_non_space = input.find_last_not_of(" \t\n\r\f\v");

    if (first_non_space != string::npos && last_non_space != string::npos)
    {
        for (size_t i = first_non_space; i < last_non_space; i += 1)
        {
            if (isspace(input[i]) && isspace(input[i + 1]))
                continue;

            vessel.push_back(input[i]);
        }
        vessel.push_back(input[last_non_space]);
    }
    return vessel;
}