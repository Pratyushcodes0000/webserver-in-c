#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>


class Route {
    public:
    void create(std::string path);
    void get(std::string& path);
    void put(std::string& path);
    void del(std::string& path);
    private:
    void remove_spaces(std::string &path);
};

extern std::vector<std::string> route_vec;