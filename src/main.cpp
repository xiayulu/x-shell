#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "./utils.h"

namespace fs = std::filesystem;

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
  std::set<std::string> builtins{"exit", "echo", "type"};

  return builtins.find(cmd) != builtins.end();
}

std::string find_cmd(std::string cmd) {
  char *path_env = std::getenv("PATH");

  if (path_env == NULL) {
    return "";
  }

  auto paths = li::split(std::string_view(path_env), ":");

  for (auto p : paths) {
    fs::path dir{p};
    fs::path file{cmd};
    fs::path full_path = dir / file;

    if (fs::exists(full_path)) {
      return full_path.string();
    }
  }

  return "";
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

  // echo
  if (cmd == "echo") {
    // remove first
    args.erase(args.begin());

    // join back as string
    std::cout << li::join(args) << std::endl;
    return;
  }

  // type
  if (cmd == "type") {
    // empty type op
    if (args.size() <= 1) {
      return;
    }

    auto op = args[1];

    // builtins
    if (is_built_in(op)) {
      std::cout << std::format("{} is a shell builtin", op) << std::endl;
      return;
    }

    // executable files
    auto op_path = find_cmd(op);
    if (op_path.size() > 0) {
      std::cout << std::format("{} is {}", op, op_path) << std::endl;
      return;
    }

    // unkown
    std::cout << std::format("{}: not found", op);
  }
}

std::string exec(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) !=
         nullptr) {
    result += buffer.data();
  }
  return result;
}

void eval_others(std::vector<std::string> args) {
  auto cmd = args[0];

  auto full_cmd = find_cmd(cmd);

  // unkown cmd
  if (full_cmd.size() == 0) {
    std::cout << std::format("{}: command not found", args[0]) << std::endl;
    return;
  }

  // executable file
  std::vector absolute_cmd{full_cmd};
  absolute_cmd.insert(absolute_cmd.end(), std::next(args.cbegin()),
                      args.cend());

  std::cout << exec(li::join(absolute_cmd).c_str());
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
