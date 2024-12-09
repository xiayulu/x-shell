#include <format>
#include <iostream>

std::string eval(std::string expression) {
  return std::format("{}: command not found", expression);
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    auto result = eval(input);
    std::cout << result << std::endl;
  }
}
