#include "helper.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  char *cwd = getcwd(NULL, 0);
  int flag = 1;
  while (flag) {
    // Handle cwd
    printf("$ ");
    char input[MAX_COMMAND_LENGTH];
    fgets(input, sizeof(input), stdin);
    // Remove trailing end-line and add null byte
    input[strcspn(input, "\n")] = '\0';
    //
    // Tokenize the input
    char *saveptr1;
    char *command_token;
    char copy_input[MAX_COMMAND_LENGTH]; // maybe we need input again
    memcpy(copy_input, input, sizeof(input));

    // Take out the command only and check
    command_token = strtok_r(copy_input, " \t", &saveptr1);
    if (built_in_command(command_token, saveptr1, &cwd, &flag)) {
      // It is built_in command
    }
    else { // check if command exists in path and executable
      char *temp = check_executable_file_in_path(command_token);
      if (!temp) {
        printf("%s: command not found", input);
        printf("\n");
      }
      else { // handle the arguement
        char *argument_array[MAX_ARGUMENT_COUNT];
        int count = 0;
        argument_array[count++] = strdup(command_token);
        char *token = strtok_r(NULL, " \t", &saveptr1);
        while (token != NULL) {
          argument_array[count++] = strdup(token);
          if (count >= MAX_ARGUMENT_COUNT) {
            perror("More than 100 argumeants!!! What are you doing?");
            exit(1);
          }
          token = strtok_r(NULL, " \t", &saveptr1);
        }
        argument_array[count] = NULL;
        pid_t pid = fork();
        if (pid == 0) { // child process
          execv(temp, argument_array);
          printf("%s: command not found\n", command_token);
          exit(1);
        }
        wait(NULL); // wait for child process

        // free allocated memory
        for (int i = 0; i < count; i++) {
          free(argument_array[i]);
        }
      }
    }
  }
  free(cwd);
  return 0;
}
