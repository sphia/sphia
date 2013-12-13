
//
// remove-files.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <str-ends-with/str-ends-with.h>
#include "remove-files.h"

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
