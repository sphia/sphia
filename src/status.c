
//
// status.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//

#include "status.h"
#include <sophia/sp.h>

int
sphia_status (sphia_t *sphia) {
  int rc = sp_recover((sp *) sphia->db);
  return rc;
}
