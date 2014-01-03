
//
// rm.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//

#include <stddef.h>
#include <string.h>
#include "rm.h"

int
sphia_rm (sphia_t *sphia, const char *key) {
  size_t size = 0;
  int rc = 0;
  int i = 0;
  int count = 0;
  char *keys[4096];

  if (NULL != key) {
    size = strlen(key) + 1;
  }

  SPHIA_DB_FOREACH(k, v, sphia->db) {
    k[sp_keysize(_c)] = '\0';

    // accepts NULL as `key` which indicates
    // a removal of every key so we need
    // to push to a key array for removal
    // after the loop
    if (NULL == key) {
      keys[count++] = k;
      continue;
    }


    if (NULL == v)
      continue;

    size_t ksize = sp_keysize(_c);

    if (size != ksize)
      ksize--;

    if (size == ksize &&
        0 == strncmp(key, k, ksize)) {

      const char *tkey = sp_key(_c);
      size_t tsize = sp_keysize(_c);

      rc = sp_destroy(_c);

      if (-1 == rc) {
        return rc;
      }
      return sp_delete(sphia->db, tkey, tsize);
    }
  }

  if (0 != _rc) {
    return _rc;
  }

  if (NULL == key) {
    // `sp_destroy(void *o)` already called
    for (i = 0; i < count; ++i) {
      if (NULL == keys[i]) continue;
      rc = sp_delete(sphia->db, keys[i], strlen(keys[i]) + 1);
      if (-1 == rc) {
        return rc;
      }
    }
  }

  return rc;
}
