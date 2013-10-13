
#include <string.h>
#include "set.h"

int
sphia_set (sphia_t *sphia, char *key, char *value) {
  int rc = 0;
  printf("%s %s\n", key, value);
  rc = sp_set(sphia->db, key, strlen(key), value, strlen(value));
  return rc;
}
