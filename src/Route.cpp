#include "../includes/Route.h"
#include <unistd.h>

std::vector<std::string> route_vec;

void Route::remove_spaces(std::string &str) {
  // beginning whitespace
  str.erase(str.begin(),
            std::find_if(str.begin(), str.end(),
                         [](unsigned char ch) { return !std::isspace(ch); }));

  // trailing whitespace
  str.erase(std::find_if(str.rbegin(), str.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            str.end());
}

void Route::create(std::string path) {
  remove_spaces(path);
  route_vec.push_back(path);
}

void Route::get(std::string& path) {}
void Route::put(std::string& path) {}
void Route::del(std::string& path) {}
