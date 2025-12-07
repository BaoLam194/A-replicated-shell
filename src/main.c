#include "helper.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  char *cwd = getcwd(NULL, 0);

  while (1) {
    // Handle cwd
    printf("$ ");
    char input[MAX_COMMAND_LENGTH];
    fgets(input, sizeof(input), stdin);
    // Remove trailing end-line and add null byte
    input[strcspn(input, "\n")] = '\0';

    // Tokenize the input
    char *saveptr1;
    char *command_token;
    char copy_input[MAX_COMMAND_LENGTH]; // maybe we need input again
    memcpy(copy_input, input, sizeof(input));

    // Take out the command only and check
    command_token = strtok_r(copy_input, " \t", &saveptr1);
    if (strcmp(command_token, "exit") == 0) { // exit command
      break;
    }
    else if (strcmp(command_token, "echo") == 0) { // echo command
      printf("%s\n", saveptr1);
    }
    else if (strcmp(command_token, "pwd") == 0) { // pwd command
      if (cwd == NULL) {
        printf("Current working directory not found");
        exit(1);
      }
      printf("%s\n", cwd);
    }
    else if (strcmp(command_token, "cd") == 0) {
      char *temp = strtok_r(NULL, " \t", &saveptr1);
      char *to = NULL;
      if (check_path_to_dir(temp, cwd, &to)) {
        free(cwd);
        if (to == NULL) // Absolute path
          cwd = strdup(temp);
        else {
          cwd = strdup(to);
          free(to);
        }
      }
      else {
        printf("cd: %s: No such file or directory\n", temp);
      }
    }
    else if (strcmp(command_token, "type") == 0) { // type command
      if (strcmp(saveptr1, "type") == 0 || strcmp(saveptr1, "exit") == 0 ||
          strcmp(saveptr1, "echo") == 0 || strcmp(saveptr1, "pwd") == 0) {
        printf("%s is a shell builtin", saveptr1);
      }
      else { // if not built in
        char *temp = check_executable_file_in_path(saveptr1);
        if (!temp)
          printf("%s: not found", saveptr1);
        else {
          printf("%s is %s", saveptr1, temp);
          free(temp); // free the allocated memory
        }
      }
      printf("\n");
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
