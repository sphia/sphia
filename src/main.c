
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sphia.h"
#include "commander.h"


// global options
struct options {
  char *path;
  char *key;
  char *value;
  int verbose;
};

static struct options opts;


#define KEY_OPT(key)                                       \
  static void key##_opt (command_t *self);                 \
  static void key##_opt (command_t *self) {                \
    opts.key = (char *) self->arg;                         \
  }


#define KEY_OPT_TRUE(key)                                  \
  static void key##_opt (command_t *self);                 \
  static void key##_opt (command_t *self) {                \
    opts.key = 1;                                          \
  }


// flag setters
KEY_OPT(path);
KEY_OPT(key);
KEY_OPT(value);

// flag booleans
KEY_OPT_TRUE(verbose);


int
main (int argc, char *argv[]) {
  command_t program;
  char *cmd = argv[1];
  char tmp[256];
  int rc = 0;
  sphia_t *sphia;

  if (NULL == cmd) {
    cmd = "";
  }

  command_init(&program, "sphia", SPHIA_VERSION);

  // reset usage
  program.usage = "<command> [<args>] [<flags>]";

  // opts
  command_option(&program, "-V", "--verbose", "enable verbose output", verbose_opt);
  command_option(&program, "-p", "--path <path>", "set the path", path_opt);
  command_option(&program, "-k", "--key <name>", "key to get", key_opt);
  command_option(&program, "-v", "--value <name>", "value to set", value_opt);

  // parse
  command_parse(&program, argc, argv);

  if (NULL == opts.path) {
    opts.path = getcwd(tmp, sizeof(tmp));
  }

  if (1 == opts.verbose) {
    printf("path set to '%s'\n", opts.path);
  }

  if (0 == strcmp("get", cmd)) {
    if (NULL == opts.key) {
      fprintf(stderr, "error: %s\n", "Missing '--key <name>' flag");
      exit(1);
    }

    sphia = sphia_new(opts.path);
    char *value = sphia_get(sphia, opts.key);

    if (NULL == value) {
      fprintf(stderr, "error: An error occured %s\n", sp_error(""));
    } else {
      printf("%s\n", value);
    }
  } else if (0 == strcmp("set", cmd)) {
    if (NULL == opts.key) {
      fprintf(stderr, "error: %s\n", "Missing '--key <name>' flag");
      exit(1);
    } else if (NULL == opts.value) {
      fprintf(stderr, "error: %s\n", "Missing '--value <value>' flag");
      exit(1);
    }

    sphia = sphia_new(opts.path);
    rc = sphia_set(sphia, opts.key, opts.value);
    if (-1 == rc) {
      fprintf(stderr, "error: An error occured setting data for key '%s' %s\n", opts.key, sp_error(sphia->db));
    }
  }

  command_free(&program);
  return 0;
}


