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

// int main()
// {
//     string file_name = "../images/test.svg";
//     ReadAndParse vessel(file_name);
//     vessel.print_data();
//     system("pause");
//     return 0;
// }
