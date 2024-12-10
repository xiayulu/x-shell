# Build your own Shell

## Handle invalid commands

### Implement solution

In this stage, you'll implement support for handling invalid commands in your shell.

Example:

```shell
$ invalid_command
invalid_command: not found
```
### Tests

The tester will execute your program like this:

```bash
./your_program.sh
```

It will then send the following command to your shell:

```bash
$ invalid_command
invalid_command: command not found
```

The tester will check whether your shell prints `<command_name>: command not found` for an unrecognized command.

### Notes

- The command name will be a random string, so the response can't be hardcoded.
- We'll handle executing "valid" commands like `echo`, `cd` etc. in later stages, this stage only deals with unrecognized commands.
- In this stage it's okay if your program exits soon after printing the `<command_name>: command not found` message. In later stages we'll check for a REPL (Read-Eval-Print Loop), i.e. whether the shell prints a new prompt after processing each command.

### Solution

Head over to your editor / IDE and implement your solution, `src/main.cpp`:

```c++
  std::string input;
  std::getline(std::cin, input);
  std::cout << input << ": command not found" << std::endl;
}
```

To pass this stage, you'll need to (a) wait for the tester to enter a command and (b) print out an error message `<command_name>`: command not found.

To read the command, use the `std::getline()` function:

```c++
std::getline(std::cin, input);
```

Once the command has been received, use `std::cout` to display the message:

```c++
std::cout << input << ": command not found" << std::endl;
```

## REPL

In this stage, you'll implement a [REPL (Read-Eval-Print Loop)](https://en.wikipedia.org/wiki/Read–eval–print_loop).

A REPL is an interactive loop that reads user input, evaluates it, prints the result, and then waits for the next input.

### Tests

The tester will execute your program like this:

```bash
./your_program.sh
```

It will then send a series of commands to your shell:

```bash
$ invalid_command_1
invalid_command_1: command not found
$ invalid_command_2
invalid_command_2: command not found
$ invalid_command_3
invalid_command_3: command not found
$
```

After each command, the tester will check if `<command_name>: command not found` is printed, and whether a prompt is printed for the next command.

### Notes

- The exact number of commands sent and the command names will be random.
- Just like the previous stages, all commands will be invalid commands, so the response will always be `<command_name>: command not found`.

### Solution

```c++
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
```

## The exit builtin

In this stage, you'll implement the [exit](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#exit) builtin.

### Tests

The tester will execute your program like this:

```bash
./your_program.sh
```

It'll then send an invalid command to your shell, followed by the `exit` command:

```bash
$ invalid_command_1
invalid_command_1: command not found
$ exit 0
```

After issuing the `exit 0` command, the tester will verify whether your program terminates with [code/status](https://en.wikipedia.org/wiki/Exit_status) 0.

### Notes

- The tester will always pass in `0` as the argument to the `exit` command.

### Solution

```c++
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
```

## The echo builtin

In this stage, you'll implement the [echo](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/echo.html) builtin.

### Tests

The tester will execute your program like this:

```bash
./your_program.sh
```

It'll then send a series of `echo` commands to your shell:

```bash
$ echo hello world
hello world
$ echo pineapple strawberry
pineapple strawberry
$
```

After each command, the tester will check if the `echo` command correctly prints the provided text back.

### Solution

```c++
  if (cmd == "echo") {
    // remove first
    args.erase(args.begin());
    // join back as string
    std::cout << li::join(args) << std::endl;
    return;
  }
```

## The type builtin: builtins

In this stage, you'll implement the `type` builtin command for your shell.

The `type` builtin is used to determine how a command would be interpreted if used. Example:

```bash
$ type echo
echo is a shell builtin
$ type exit
exit is a shell builtin
$ type invalid_command
invalid_command: not found
```

In this stage we'll only test two cases: builtin commands and unrecognized commands. We'll handle executable files in later stages.

### Tests

The tester will execute your program like this:

```bash
./your_program.sh
```

It'll then send a series of `type` commands to your shell:

```bash
$ type echo
echo is a shell builtin
$ type exit
exit is a shell builtin
$ type type
type is a shell builtin
$ type invalid_command
invalid_command: not found
$
```

The tester will check if the `type` command responds correctly based on the command provided:

- If a command is a shell builtin, the expected output is `<command> is a shell builtin`.
- If a command is not recognized, the expected output is `<command>: not found`.

### Notes

- The tester will only check for builtin commands and unrecognized commands in this stage.
- `type` itself is a shell builtin command, so `$ type type` should print `type is a shell builtin`.

### Solution

```c++
// type
  if (cmd == "type") {
    // empty type op
    if (args.size() <= 1) {
      return;
    }

    auto op = args[1];

    if (is_built_in(op)) {
      std::cout << std::format("{} is a shell builtin", op) << std::endl;
    } else {
      std::cout << std::format("{}: not found", op) << std::endl;
    }
  }
```

## The type builtin: executable files

In this stage, you'll extend the `type` builtin to search for executable files using [PATH](https://en.wikipedia.org/wiki/PATH_(variable)).

[PATH](https://en.wikipedia.org/wiki/PATH_(variable)) is an environment variable that specifies a set of directories where executable programs are located. When a command is received, the  program should search for the command in the directories listed in the PATH environment variable. If the command is  found, the program should print the path to the command. If the command  is not found, the program should print `<command>: not found`.

### Tests

The tester will execute your program with a custom `PATH` like this:

```bash
PATH="/usr/bin:/usr/local/bin" ./your_program.sh
```

It'll then send a series of `type` commands to your shell:

```bash
$ type ls
ls is /usr/bin/ls
$ type abcd
abcd is /usr/local/bin/abcd
$ type invalid_command
invalid_command: not found
$
```

The tester will check if the `type` command correctly identifies executable files in the PATH.

### Notes

- The actual value of the `PATH` environment variable will be random for each test case.
- `PATH` can contain multiple directories separated by colons (`:`), your program should search for programs in each directory in order and return the first match.
