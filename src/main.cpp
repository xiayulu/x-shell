#include <format>
#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::cout << "$ ";

  std::string input;
  std::getline(std::cin, input);

  std::cout << std::format("{}: command not found", input) << std::endl;
}
