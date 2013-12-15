
//
// purge.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <str-ends-with/str-ends-with.h>
#include "file.h"
#include "purge.h"

static char *extensions[] = {".log"};

int
sphia_purge (const char *path) {
  return remove_files(path, extensions, 1);
}
