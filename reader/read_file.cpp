#include "read_file.h"

ReadAndParse::ReadAndParse(const string &file_name)
{
    this->file_name = file_name;
    this->read_file(file_name);
}

void ReadAndParse::read_file(const string &file_name)
{
    xml_document<> document;
    xml_node<> *root_node;
    ifstream file(file_name);
    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    document.parse<0>(&buffer[0]);
    root_node = document.first_node();
    for (xml_node<> *node = root_node->first_node(); node != nullptr; node = node->next_sibling())
    {
        Entity entity;
        entity.entity_type = node->name();
        for (xml_attribute<> *attribute = node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
            entity.attributes[attribute->name()] = attribute->value();
        if (node->value())
            entity.content = node->value();
        entities.push_back(entity);
    }
}

vector<Entity> ReadAndParse::get_data()
{
    return this->entities;
}

void ReadAndParse::print_data()
{
    for (int i = 0; i < entities.size(); i += 1)
    {
        const Entity &current_entity = entities[i];
        cout << "#" << i << " " << current_entity.entity_type << endl;
        for (const auto &attribute : current_entity.attributes)
            cout << attribute.first << ": " << attribute.second << endl;
        if (!current_entity.content.empty())
            cout << "content: " << current_entity.content << endl;
        cout << endl;
    }
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
