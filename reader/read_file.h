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

struct Transform
{
    string type;
    vector<float> values;
};

struct Entity
{
    int depth = 0;
    string type;
    string text_content;
    unordered_map<string, string> attributes;
    vector<Entity> children;
};

class SvgParser
{
private:
    Entity data;
    Entity defs;
    string filename;

public:
    SvgParser(const string &);
    Entity get_data();
    Entity get_defs();
    void traverse_node(const xml_node<> *, Entity &, int = 0);
    void traverse_defs(const xml_node<> *, Entity &, int = 0);
    void parse_file();
    void print_data() const;
};

string to_lower(const string &);
string replace_comma_with_space(const string &);
string add_space_between_char_and_number(const string &);
string format_text(const string &);
string remove_percent(const string &);

extern string filename;
extern SvgParser svg_parser;
extern Entity svg_data;
#endif
