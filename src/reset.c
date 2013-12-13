
//
// reset.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <str-ends-with/str-ends-with.h>
#include "purge.h"
#include "reset.h"

static int
handle_reset (const char *);

int
sphia_reset (const char *path) {
  int rc = sphia_purge(path);

  if (0 == rc) {
    return handle_reset(path);
  } else {
    return handle_reset(path);
  }
}


static int
handle_reset (const char *path) {
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
    } else if (0 == str_ends_with(fd->d_name, ".log") &&
               0 == str_ends_with(fd->d_name, ".incomplete")) {
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
