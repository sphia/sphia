
//
// sphia.h
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#ifndef __SPHIA_H__
#define __SPHIA_H__ 1

#include <stdio.h>
#include <sophia/sophia.h>
#include <sophia/sp.h>


#define SPHIA_VERSION "0.0.9"

#define SPHIA_DB_FOREACH(_k, _v, _db)                      \
  char *_k = NULL, *_v = NULL;                             \
  void *_c = sp_cursor(_db, SPGT, NULL, 0);                \
  int _s = 0, _rc = 0;                                     \
  if (NULL == _c) { _s = 1; (_rc = sp_destroy(_c)); }      \
  while ((0 == _s && sp_fetch(_c) &&                       \
        (_k = (char *) sp_key(_c)) &&                      \
        (_v = (char *) sp_value(_c)))                      \
        || (_rc = sp_destroy(_c)))

#define sphia_error(s) {                                   \
  fprintf(stderr, "error: %s\n", s);                       \
}

#define sphia_ferror(fmt, ...) {                           \
  char s[256];                                             \
  sprintf(s, "error: %s\n", fmt);                          \
  fprintf(stderr, s, ##__VA_ARGS__);                       \
}


#define sphia_db_error(fmt, ...) {                         \
  char s[256];                                             \
  sprintf(s, "%s\n", fmt);                                 \
  fprintf(stderr, s, ##__VA_ARGS__);                       \
}


typedef struct {
  char *path;
  void *db;
  void *env;
} sphia_t;


sphia_t *
sphia_new (char *path);


void
sphia_free (sphia_t *sphia);


#endif

