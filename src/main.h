
//
// main.h
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#ifndef SPHIA_MAIN_H
#define SPHIA_MAIN_H 1

#include <commander/commander.h>
#include "options.h"
#include "sphia.h"

// Function pointer to define functions for commands.
typedef int (*cmd_func_t)(sphia_t *);

// Defines a specific command and a function to call for it.
typedef struct {
  char *name;
  char *alt;
  int open;
  cmd_func_t func;
} cmd_t;

static options_t opts;

#ifdef _WIN32
#define DEFAULT_OPT_ENV "APPDATA"
#else
#define DEFAULT_OPT_ENV "HOME"
#endif


// Creates a function that sets the key with the given
// command value.
#define KEY_OPT(key)                                       \
  static void key##_opt (command_t *);                     \
  static void key##_opt (command_t *self) {                \
    opts.key = (char *) self->arg;                         \
  }

// Creates a function that sets a boolean option to true.
#define KEY_OPT_BOOL(key)                                  \
  static void key##_opt (command_t *);                     \
  static void key##_opt (command_t *self) {                \
    self->arg = "";                                        \
    opts.key = 1;                                          \
  }

// Generates a command function definition.
#define GEN_CMD(name)                                      \
  static int cmd_##name (sphia_t *);                       \
  static int cmd_##name (sphia_t *sphia)

#endif
