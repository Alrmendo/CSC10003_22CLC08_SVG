#ifndef __READ_FILE__
#define __READ_FILE__
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include "../libraries/rapidxml.hpp"

using namespace rapidxml;
using namespace std;

struct Entity
{
    string entity_type = "";
    unordered_map<string, string> attributes;
    string content = "";
};

class ReadAndParse
{
private:
    vector<Entity> entities;
    string file_name;

public:
    ReadAndParse(const string &file_name);
    void read_file(const string &file_name);
    vector<Entity> get_data();
    void print_data();
};
#endif
