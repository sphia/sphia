
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
  size_t size = sizeof(key);
  size_t vsize;
  char *value;
  int rc;

  rc = sp_get(sphia->db, key, size, &value, &vsize); 

  if(rc == 1) {
  	return value;
  }else {
  	return NULL;
  }
}
