#include "helper.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

const char *BUILT_IN_COMMAND[] = {"echo", "pwd", "cd", "exit", "type"};
const int BUILT_IN_SIZE = sizeof(BUILT_IN_COMMAND) / sizeof(BUILT_IN_COMMAND[0]);

char *command_generator(const char *text, int state) {
  static int index, len;
  if (state == 0) {
    index = 0;
    len = strlen(text);
  }
  while (index < BUILT_IN_SIZE) {
    const char *name = BUILT_IN_COMMAND[index++];
    if (strncmp(name, text, len) == 0) {
      return strdup(name);
    }
  }

  return NULL;
}
char **command_completion(const char *text, int start, int end) {
  return rl_completion_matches(text, command_generator);
}