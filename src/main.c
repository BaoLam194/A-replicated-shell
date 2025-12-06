#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while (1) {
    printf("$ ");
    char input[512];
    fgets(input, sizeof(input), stdin);
    // Remove trailing end-line and add null byte
    input[strcspn(input, "\n")] = '\0';

    // Tokenize the input
    char *saveptr1;
    char *command_token;
    char copy_input[512];
    memcpy(copy_input, input, sizeof(input));
    command_token = strtok_r(copy_input, " \t", &saveptr1);
    if (strcmp(command_token, "exit") == 0)
      break;
    else if (strcmp(command_token, "echo") == 0) {
      printf("%s\n", saveptr1);
    } else
      printf("%s: command not found\n", input);
  }
  return 0;
}
