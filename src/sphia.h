
//
// sphia.h
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#ifndef SPHIA_H
#define SPHIA_H 1

#include <stdio.h>
#include <sophia/sophia.h>
#include <sophia/sp.h>

#define SPHIA_VERSION "0.0.9"

// Loop through each key in the given db in a while loop,
// the given _k, and _v variables identify the current key
// and value. _rc contains the return value for the loop,
// -1 indicates an error. Do not free the key or value.
#define SPHIA_DB_FOREACH(_k, _v, _db)                      \
  char *_k = NULL, *_v = NULL;                             \
  void *_c = sp_cursor(_db, SPGT, NULL, 0);                \
  int _s = 0, _rc = 0;                                     \
  if (NULL == _c) { _s = 1; (_rc = sp_destroy(_c)); }      \
  while ((0 == _s && sp_fetch(_c) &&                       \
        (_k = (char *) sp_key(_c)) &&                      \
        (_v = (char *) sp_value(_c)))                      \
        || (_rc = sp_destroy(_c)))

// Print an message to stderr, prefixed with "error:".
#define sphia_error(s) {                                   \
  fprintf(stderr, "error: %s\n", s);                       \
}

// Print a formatted message to stderr, prefixed with "error:".
#define sphia_ferror(fmt, ...) {                           \
  char s[256];                                             \
  sprintf(s, "error: %s\n", fmt);                          \
  fprintf(stderr, s, ##__VA_ARGS__);                       \
}

// Print a formatted message to stderr.
#define sphia_db_error(fmt, ...) {                         \
  char s[256];                                             \
  sprintf(s, "%s\n", fmt);                                 \
  fprintf(stderr, s, ##__VA_ARGS__);                       \
}

// Defines a database instance and the path it points to.
typedef struct {
  const char *path;
  void *db;
  void *env;
} sphia_t;

// Create a new database instance from the given path, returns NULL on error.
sphia_t *
sphia_new (const char *);

// Frees the database memory.
void
sphia_free (sphia_t *);

#endif

