
//
// set.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#include <string.h>
#include "set.h"

int
sphia_set (sphia_t *sphia, const char *key, const char *value) {
  return sp_set(sphia->db, key, sizeof(key), value, sizeof(value));
}
