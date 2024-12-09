#include <cstdlib>
#include <format>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> parse_cmd(std::string input) {
  std::stringstream ss{input};
  std::string sp;

  std::vector<std::string> args;

  // split input by ' '
  while (std::getline(ss, sp, ' ')) {
    args.push_back(sp);
  }

  return args;
}

bool is_built_in(std::string cmd) {
  std::set<std::string> builtins{"exit"};

  return builtins.find(cmd) != builtins.end();
}

void eval_builtins(std::vector<std::string> args) {
  auto cmd = args[0];

  // exit
  if (cmd == "exit") {
    int code = 0;
    if (args.size() > 1) {
      code = std::stoi(args[1]);
    }

    exit(code);
  }
}

void eval_others(std::vector<std::string> args) {
  auto cmd = args[0];

  // unkown cmd
  std::cout << std::format("{}: command not found", args[0]) << std::endl;
}

void eval(std::string input) {
  auto args = parse_cmd(input);

  // empty intput
  if (args.size() == 0) {
    return;
  }

  // handle builtins
  if (is_built_in(args[0])) {
    eval_builtins(args);
  } else {
    eval_others(args);
  }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    eval(input);
  }
}
