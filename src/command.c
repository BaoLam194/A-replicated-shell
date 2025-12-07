#include "helper.h"
char *check_executable_file_in_path(const char *file) {
  // Get path variable and split
  char *split, *savepath;
  char *path = strdup(getenv("PATH"));
  split = strtok_r(path, PATH_LIST_SEPARATOR, &savepath);
  while (split != NULL) { // Check for each seperate directory
    char temp[500] = "";
    strcat(temp, split);
    strcat(temp, "/");
    strcat(temp, file);
    if (access(temp, F_OK | X_OK) == 0) {
      free(path);
      char *result = strdup(temp); // Allocate memory for return T_T
      return result;
    }
    split = strtok_r(NULL, PATH_LIST_SEPARATOR, &savepath);
  }

  free(path);
  return NULL; // CANNOT find the file or no executable permission
}

bool check_path_to_dir(const char *path, const char *from) {
  if (path[0] == '/') { // Absolute path
    if (access(path, X_OK) != 0) {
      // can not access (could be no execute permission
      // or path not exist)
      return false;
    }
    struct stat sb;
    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
      // path exists and is directory
      return true;
    }
  }
  return false;
}