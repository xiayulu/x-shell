#include <algorithm>
#include <ranges>
#include <string_view>

#include "./utils.h"

namespace li {

std::string join(std::vector<std::string> vec, std::string delimiter) {
  if (vec.empty()) {
    return "";
  }

  return std::ranges::fold_left(vec | std::views::filter([](std::string s) {
                                  return !s.empty();
                                }) | std::views::join_with(delimiter),
                                std::string(), std::plus());
};

std::vector<std::string> split(std::string_view s, std::string_view delimiter) {
  int start, end = -1 * delimiter.size();
  std::vector<std::string> result;

  do {
    start = end + delimiter.size();
    end = s.find(delimiter, start);
    result.push_back(std::string{s.substr(start, end - start)});
  } while (end != -1);

  return result;
};


// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}
} // namespace li
