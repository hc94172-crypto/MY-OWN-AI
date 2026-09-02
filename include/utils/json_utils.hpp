#ifndef JSON_UTILS_HPP
#define JSON_UTILS_HPP

#include "../../third_party/httplib.h"

#include <string>
#include <vector>

std::string jS(const std::string& s);

std::string jVec(const std::vector<float>& v);

std::vector<float> parseVec(const std::string& s);

std::string extractStr(
    const std::string& body,
    const std::string& key);

int extractInt(
    const std::string& body,
    const std::string& key,
    int def = 0);

bool parseBody(
    const std::string& body,
    std::string& metadata,
    std::string& category,
    std::vector<float>& embedding);

void cors(httplib::Response& res);

#endif