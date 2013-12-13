
//
// get.h
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#ifndef SPHIA_GET_H
#define SPHIA_GET_H 1

#include "sphia.h"

// Gets the value for a specified key, returns NULL if none found or on errors. Do
// not free the returned string.
char *
sphia_get (sphia_t *, const char *);

#endif

