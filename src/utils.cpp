#include <algorithm>
#include <ranges>

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

} // namespace li
