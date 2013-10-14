
//
// sphia.h
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#ifndef __SPHIA_H__
#define __SPHIA_H__ 1


#include <stdio.h>
#include <stdlib.h>
#include <sophia.h>


#define SPHIA_VERSION "0.0.1"

#define sphia_error(s) {                                   \
  fprintf(stderr, "error: %s\n", s);                       \
}

#define sphia_ferror(fmt, ...) {                           \
  char s[256];                                             \
  sprintf(s, "error: %s\n", fmt);                          \
  fprintf(stderr, s, ##__VA_ARGS__);                       \
}


typedef struct {
  char *path;
  void *db;
  void *env;
} sphia_t;


sphia_t *
sphia_new (char *path);


void
sphia_free (sphia_t *sphia);

#include "db.h"
#include "get.h"
#include "set.h"
#include "rm.h"

#endif

