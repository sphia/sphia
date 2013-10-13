
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


typedef struct {
  char *path;
  void *db;
  void *env;
} sphia_t;


sphia_t *
sphia_new (char *path);


#include "db.h"
#include "get.h"
#include "set.h"


#endif

