
//
// purge.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <str-ends-with/str-ends-with.h>
#include "purge.h"

static inline int
handle_purge (char *path);

int
sphia_purge (char *path) {
  return handle_purge(path);
}


static inline int
handle_purge (char *path) {
  DIR *dir = opendir(path);
  char file[1024];
  struct dirent *fd;
  int rc = 0;

  if (NULL == dir) {
    return -1;
  }

  while ((fd = readdir(dir))) {
    if ('.' == fd->d_name[0]) {
      continue;
    } else if (str_ends_with(fd->d_name, ".log")) {
      continue;
    }

    snprintf(file, sizeof(file), "%s/%s", path, fd->d_name);

    rc = unlink(file);

    if (-1 == rc) {
      closedir(dir);
      return -1;
    }
  }

  closedir(dir);
  return rc;
}
