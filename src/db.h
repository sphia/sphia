
//
// db.h
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#ifndef __SPHIA_DB_H__
#define __SPHIA_DB_H__ 1

#include <sophia.h>
#include <sp.h>
#include "sphia.h"


#define SPHIA_DB_FOREACH(_k, _v, _db)                      \
  char *_k = NULL, *_v = NULL;                             \
  void *_c = sp_cursor(_db, SPGT, NULL, 0);                \
  int _s = 0;                                              \
  if (NULL == _c) { _s = 1; sp_destroy(_c); }              \
  while (0 == _s && sp_fetch(_c) &&                        \
          (_k = (char *) sp_key(_c)) &&                    \
          (_v = (char *) sp_value(_c))                     \
        || sp_destroy(_c))

#endif
