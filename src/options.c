
//
// options.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inih/ini.h>
#include "options.h"

static int freePath = 0;

// Handles each option in an ini file, first argument being the struct
// we're marshalling, then the section, the name for the value, and finally
// the value.
static int
handler(void *, const char *, const char *, const char*);

int
read_options(options_t *opts, const char *path, int required) {
  FILE *file = fopen(path, "r");
  if (NULL == file) {
    if (ENOENT == errno && 0 == required) {
      return 0;
    }

    return -1;
  }

  int rc = ini_parse_file(file, handler, opts);

  fclose(file);
  if (0 != rc) {
    // ini_parse_file returns multiple negative numbers on error.
    return -1;
  }

  return 0 == freePath ? 0 : 1;
}

static int
handler (void *opts_g, const char *section, const char *name, const char *value) {
  options_t *opts = (options_t *)opts_g;

  #define MATCH(s, n) 0 == strcmp(section, s) && 0 == strcmp(name, n)
  if (MATCH("", "path")) {
    char *path = strdup(value);
    if (NULL == path) {
      return 0;
    }

    freePath = 1;
    opts->path = path;
  } else if (MATCH("", "verbose")) {
    opts->verbose = atoi(value);
  } else if (MATCH("", "strict")) {
    opts->strict = atoi(value);
  }

  return 1;
}
