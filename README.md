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

