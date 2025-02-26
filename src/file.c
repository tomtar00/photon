#include "internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

bool file_exists(const char *path) {
  if (path == 0)
    return false;
  struct stat buffer;
  return stat(path, &buffer) == 0;
}

char *load_file(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    PH_ERROR("Failed to open file '%s'", filename);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc(fileSize + 1);
  if (!buffer) {
    PH_ERROR("Failed to allocate memory");
    fclose(file);
    return NULL;
  }

  fread(buffer, 1, fileSize, file);
  buffer[fileSize] = '\0';

  fclose(file);
  return buffer;
}