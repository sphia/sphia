
#include "get.h"


char *
sphia_get (sphia_t *sphia, char *key) {
  int size = (int) strlen(key);

  SPHIA_DB_FOREACH(k, v, sphia->db) {
    if (NULL == v) continue;
    int ksize = (int) strlen(k);
    if (size != ksize) ksize--;
    if (size == ksize) {
      if (0 == strncmp(key, k, ksize) &&
          0 == strncmp(k, key, size)) {
        sp_destroy(_c);
        return v;
      }
    }
  }

  return NULL;
}
