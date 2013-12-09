
//
// clear.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#include "rm.h"
#include "clear.h"

int
sphia_clear (sphia_t *sphia) {
  return sphia_rm(sphia, NULL);
}
