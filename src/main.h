
//
// main.h
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#ifndef SPHIA_MAIN_H
#define SPHIA_MAIN_H 1

#include <commander/commander.h>
#include "sphia.h"

typedef struct {
  char *path;
  char *key;
  char *value;
  int verbose;
  int strict;
} options_t;

typedef int (*cmd_func_t)(sphia_t *);

typedef struct {
  char *name;
  char *alt;
  int open;
  cmd_func_t func;
} cmd_t;

#define KEY_OPT(key)                                       \
  static void key##_opt (command_t *);                     \
  static void key##_opt (command_t *self) {                \
    opts.key = (char *) self->arg;                         \
  }

#define KEY_OPT_BOOL(key)                                  \
  static void key##_opt (command_t *);                     \
  static void key##_opt (command_t *self) {                \
    self->arg = "";                                        \
    opts.key = 1;                                          \
  }

#define GEN_CMD(name)                                      \
  static int cmd_##name (sphia_t *);                       \
  static int cmd_##name (sphia_t *sphia)

#endif
