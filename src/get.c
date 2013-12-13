
//
// get.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//

#include <stddef.h>
#include <string.h>
#include "get.h"

char *
sphia_get (sphia_t *sphia, const char *key) {
  size_t size = strlen(key);

  SPHIA_DB_FOREACH(k, v, sphia->db) {
    k[sp_keysize(_c)] = '\0';
    if (NULL == v) {
      continue;
    }
    size_t ksize = strlen(k);

    if (size == ksize) {
      if (0 == strncmp(key, k, ksize) && 0 == strncmp(k, key, size)) {
        sp_destroy(_c);
        return v;
      }
    }
  }

  return NULL;
}
