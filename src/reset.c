
//
// reset.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <str-ends-with/str-ends-with.h>
#include "file.h"
#include "purge.h"
#include "reset.h"

static char *extensions[] = {".log", ".incomplete"};

int
sphia_reset (const char *path) {
  int rc = sphia_purge(path);

  if (0 == rc) {
    return remove_files(path, extensions, 2);
  } else {
    return remove_files(path, extensions, 2);
  }
}
