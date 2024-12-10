#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace li {

std::string join(std::vector<std::string> vec, std::string delimiter = " ");

std::vector<std::string> split(std::string_view s,
                               std::string_view delimiter = " ");

inline void ltrim(std::string &s);

inline void rtrim(std::string &s);

inline void trim(std::string &s);
} // namespace li