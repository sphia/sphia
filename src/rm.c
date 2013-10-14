
#include "rm.h"

int
sphia_rm (sphia_t *sphia, char *key) {
  int size = (int) strlen(key);
  int rc = 0;

  SPHIA_DB_FOREACH(k, v, sphia->db) {
    if (NULL == v)
      continue;

    int ksize = (int) strlen(k);

    if (size != ksize)
      ksize--;

    if (size == ksize &&
        0 == strncmp(key, k, ksize) &&
        0 == strncmp(k, key, size)) {

      const char *tkey = sp_key(_c);
      size_t tsize = sp_keysize(_c);

      rc = sp_destroy(_c);

      if (-1 == rc) {
        return rc;
      } else {
        return sp_delete(sphia->db, tkey, tsize);
      }
    }
  }

  return rc;
}
