#ifndef __READ_FILE__
#define __READ_FILE__

#include <iostream>
#include <vector>
#include <cctype>
#include <string>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include "../libraries/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

struct Entity
{
    int depth;
    string type;
    string text_content;
    unordered_map<string, string> attributes;
    vector<Entity> children;
};

class SvgParser
{
private:
    Entity data;
    string filename;

public:
    SvgParser(const string &filename);
    Entity get_data();
    void traverse_node(const xml_node<> *node, Entity &entity, int depth = 0);
    void parse_file();
    void print_data() const;
};

string to_lower(const string &input);
string replace_comma_with_space(const string &input);
string add_space_between_char_and_number(const string &input);
string format_text(const string &input);
#endif
