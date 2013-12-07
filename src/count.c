
//
// count.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#include "count.h"

int
sphia_count (sphia_t *sphia) {
  int count = 0;

  SPHIA_DB_FOREACH(k, v, sphia->db) {
    if (NULL != v) {
      count++;
    }
  }

  // from macro
  if (0 != _rc) {
    return _rc;
  }

  return count;
}
