
//
// get.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#include "get.h"

char *
sphia_get (sphia_t *sphia, char *key) {
  int size = (int) strlen(key);

  // trim new lines and etc
  key = trim(key);

  SPHIA_DB_FOREACH(k, v, sphia->db) {
    k[sp_keysize(_c)] = '\0';

    if (NULL == v)
      continue;

    int ksize = (int) strlen(k);

    if (size != ksize)
      ksize--;

    if (size == ksize) {
      if (0 == strncmp(key, k, ksize) && 0 == strncmp(k, key, size)) {
        sp_destroy(_c);
        return v;
      }
    }
  }

  return NULL;
}
