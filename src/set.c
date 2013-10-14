
//
// set.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#include <string.h>
#include "set.h"

int
sphia_set (sphia_t *sphia, char *key, char *value) {
  int rc = 0;

  // trim new lines and etc
  key = trim(key);
  value = trim(value);
  rc = sp_set(sphia->db, key, strlen(key), value, strlen(value));
  return rc;
}
