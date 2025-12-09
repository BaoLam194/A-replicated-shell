#include "helper.h"

void echo_command(char *argument) { printf("%s ", argument); }
void type_command(char *argument) {
  if (strcmp(argument, "type") == 0 || strcmp(argument, "exit") == 0 ||
      strcmp(argument, "echo") == 0 || strcmp(argument, "pwd") == 0 ||
      strcmp(argument, "cd") == 0) {
    printf("%s is a shell builtin", argument);
  }
  else { // if not built in
    char *temp = check_executable_file_in_path(argument);
    if (!temp)
      printf("%s: not found", argument);
    else {
      printf("%s is %s", argument, temp);
      free(temp); // free the allocated memory
    }
  }
  printf("\n");
}

bool built_in_command(char **command, char *argument_str, int count,
                      char **cwd) {

  char *command_token = command[0];
  if (strcmp(command_token, "exit") == 0) { // exit command
    exit(0);
    return true;
  }
  else if (strcmp(command_token, "echo") == 0) { // echo command
    for (int i = 1; i < count; i++) {
      echo_command(command[i]);
    }
    printf("\n");
    return true;
  }
  else if (strcmp(command_token, "pwd") == 0) { // pwd command
    if (*cwd == NULL) {
      printf("Current working directory not found");
      exit(1);
    }
    printf("%s\n", *cwd);
    return true;
  }
  else if (strcmp(command_token, "cd") == 0) {
    if (count > 2) {
      printf("%s: too many arguments", command_token);
      return true;
    }
    char *temp = command[1];
    char *to = NULL;
    if (check_path_to_dir(temp, *cwd, &to)) {
      free(*cwd);
      if (to == NULL) // Absolute path
        *cwd = strdup(temp);
      else {
        *cwd = strdup(to);
        free(to);
      }
    }
    else {
      printf("cd: %s: No such file or directory\n", temp);
    }
    return true;
  }
  else if (strcmp(command_token, "type") == 0) { // type command
    for (int i = 1; i < count; i++) {
      type_command(command[i]);
    }
    return true;
  }
  return false;
}

// Need to modify the saveptr so **
void existing_command(char **command, int count) {
  char *command_token = command[0];
  char *temp = check_executable_file_in_path(command_token);
  if (!temp) {
    printf("%s: command not found", command_token);
    printf("\n");
  }
  else { // handle the arguement
    char **argument_list = malloc(sizeof(char *) * (count + 1));
    for (int i = 0; i < count; i++) {
      argument_list[i] = strdup(command[i]);
    }
    argument_list[count] = NULL;
    pid_t pid = fork();
    if (pid == 0) { // child process
      execv(temp, argument_list);
      printf("%s: command not found\n", argument_list[0]);
      exit(1);
    }
    wait(NULL); // wait for child process
    for (int i = 0; i < count; i++) {
      free(argument_list[i]);
    }
    free(argument_list);
  }
}