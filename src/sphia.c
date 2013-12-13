
//
// sphia.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#include <stdlib.h>
#include <assert.h>
#include "sphia.h"

sphia_t *
sphia_new (const char *path) {
  int rc = 0;
  sphia_t *sphia = malloc(sizeof(sphia_t));
  if (NULL == sphia) return NULL;
  sphia->path = path;
  sphia->env = sp_env();
  assert(sphia->env);
  rc = sp_ctl(sphia->env, SPDIR, SPO_CREAT|SPO_RDWR, path);

  if (-1 == rc) {
    sphia_error("Error creating or reading database");
    sphia_db_error("%s", sp_error(sphia->env));
    sp_destroy(sphia->env);
    free(sphia);
    return NULL;
  }

  rc = sp_ctl(sphia->env, SPGC, 1);

  if (-1 == rc) {
    sphia_db_error("%s", sp_error(sphia->env));
    sp_destroy(sphia->env);
    free(sphia);
    return NULL;
  }

  sphia->db = sp_open(sphia->env);

  if (NULL == sphia->db) {
    sphia_db_error("%s", sp_error(sphia->env));
    sp_destroy(sphia->env);
    free(sphia);
    return NULL;
  }

  return sphia;
}

void
sphia_free (sphia_t *sphia) {
  sp_destroy(sphia->db);
  sp_destroy(sphia->env);
  free(sphia);
}
