
//
// file.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <str-ends-with/str-ends-with.h>
#include "file.h"

int
remove_files (const char *path, char *extensions[], int len) {
  DIR *dir = opendir(path);
  char file[1024];
  struct dirent *fd;
  int rc = 0;
  int i = 0;

  if (NULL == dir) {
    return -1;
  }

  while ((fd = readdir(dir))) {
    int skip = 0;

    if ('.' == fd->d_name[0]) {
      skip = 1;
    } else {
      for (i = 0; i < len; i++) {
        if (str_ends_with(fd->d_name, extensions[i])) {
          skip = 1;
          break;
        }
      }
    }

    if (1 == skip) {
      continue;
    }

    snprintf(file, sizeof(file), "%s/%s", path, fd->d_name);

    rc = unlink(file);
    if (-1 == rc) {
      closedir(dir);
      return -1;
    }
  }

  rc = closedir(dir);
  return rc;
}

int
file_exists (const char *path) {
  struct stat *stats = malloc(sizeof(struct stat));
  int rc = 0;
  if (NULL == stats) {
    return -1;
  }

  rc = stat(path, stats);
  free(stats);
  if (-1 == rc && ENOENT == errno) {
    return 0;
  }

  return 1;
}
