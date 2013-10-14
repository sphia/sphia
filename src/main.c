
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

  if (0 == strcmp("init", cmd)) {

    //
    // $ sphia init --path ~/db
    //

    sphia = sphia_new(opts.path);

    if (NULL == sphia) {
      sphia_ferror("Error initializing sophia database to path '%s'", opts.path);
      sphia_free(sphia);
      exit(1);
    }

    sphia_free(sphia);

  } else if (0 == strcmp("get", cmd)) {

    //
    // $ sphia get --key foo --path ~/db
    //

    if (NULL == opts.key) {
      sphia_ferror("%s", "Missing '--key <name>' flag");
      exit(1);
    }

    sphia = sphia_new(opts.path);
    char *value = sphia_get(sphia, opts.key);

    if (NULL == value) {
      char *msg = sp_error(sphia->db);

      if (NULL != msg) {
        sphia_ferror("An error occured. %s", msg);
        sphia_free(sphia);
        exit(1);
      } else {
        sphia_ferror("Couldn't find '%s'", opts.key);
        sphia_free(sphia);
        exit(1);
      }
    } else {
      printf("%s\n", value);
    }

  } else if (0 == strcmp("set", cmd)) {

    //
    // $ sphia set --key foo --value bar --path ~/db
    //

    if (NULL == opts.key) {
      sphia_ferror("%s", "Missing '--key <name>' flag");
      exit(1);
    } else if (NULL == opts.value) {
      sphia_ferror("%s", "Missing '--value <value>' flag");
      exit(1);
    }

    sphia = sphia_new(opts.path);
    rc = sphia_set(sphia, opts.key, opts.value);

    if (-1 == rc) {
      sphia_ferror("An error occured setting data for key '%s' %s", opts.key, sp_error(sphia->db));
      sphia_free(sphia);
      exit(1);
    } else {
      printf("%s = '%s'\n", opts.key, opts.value);
      sphia_free(sphia);
    }

  } else if (0 == strcmp("list", cmd) || 0 == strcmp("ls", cmd)) {

    //
    // $ sphia list --path ~/db
    //

    sphia = sphia_new(opts.path);

    if (NULL == sphia) {
      sphia_error("Failed to open sophia database");
      sphia_free(sphia);
      exit(1);
    }

    SPHIA_DB_FOREACH(key, val, sphia->db) {
      printf("%s => '%s'\n", key, val);
    }

    sphia_free(sphia);

  } else if (0 == strcmp("rm", cmd) || 0 == strcmp("remove", cmd)) {

    //
    // $ sphia rm --key name --path ~/db
    //

    sphia = sphia_new(opts.path);

    if (NULL == sphia) {
      sphia_error("Failed to open sophia database");
      sphia_free(sphia);
      exit(1);
    }

    rc = sphia_rm(sphia, opts.key);

    if (-1 == rc) {
      sphia_ferror("Error removing key '%s'. %s", opts.key, sp_error(sphia->db));
      sphia_free(sphia);
      exit(1);
    }

    sphia_free(sphia);
  }

  command_free(&program);
  return 0;
}


