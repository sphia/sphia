
//
// options.h
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#ifndef SPHIA_OPTIONS_H
#define SPHIA_OPTIONS_H 1

// Defines various command line options.
typedef struct {
  char *config;
  char *path;
  char *key;
  char *value;
  int verbose;
  int strict;
} options_t;

// Reads the given options file and parses it into the options pointer, if
// the given int is 1 then the file is required to exist and will error out
// if it doesn't. Returns -1 on error.
int
read_options(options_t *, const char *, int);

#endif

