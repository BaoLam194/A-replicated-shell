#include "helper.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  initialize(); // set thing up for readline etc
  char *cwd = getcwd(NULL, 0);
  using_history();
  while (1) {
    // Maybe handle cwd
    char *input = readline("$ ");

    // Check for EOF.
    if (!input)
      break;
    // Add input to readline history.
    add_history(input);
    // Tokenize the input
    int count = 0, flag = 0; // currently the flag is for redirection, may
                             // change if there is better way to handle
    char **mod_input = parse_input(input, &count, &flag);
    if (!count) {
      fprintf(stderr, "No arguments provided \?\?\?\n");
      return 0;
    }
    if (!mod_input) {
      continue;
    }
    // Check which type of command for better handle
    handle_command(mod_input, count, &cwd, flag);
    for (int i = 0; i < count; i++) {
      free(mod_input[i]);
    }
    free(mod_input);
  }
  free(cwd);
  return 0;
}
