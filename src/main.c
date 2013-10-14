
//
// main.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "which.h"
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


char *
str_flatten (char *array[], int start, int end);


int
main (int argc, char *argv[]) {
  command_t program;
  char *cmd = strdup(argv[1]);
  char tmp[256];
  char *default_path = getenv("SPHIA_PATH");
  int rc = 0;
  sphia_t *sphia;

  if (NULL == cmd) {
    cmd = "";
  }

  command_init(&program, "sphia", SPHIA_VERSION);

  // reset usage
  program.usage =
    "<command> [--key <key>] [--value <value>] [--path <path>]\n"
    "                       [--version] [--help]"

    "\n\n"
    "commands:\n"
    "   init                         initialize a new database\n"
    "   get -k <key>                 get a value by key\n"
    "   set -k <key> -v <value>      set a value by key\n"
    "   ls                           list all keys and values\n"
    "   clear                        clears database of all keys\n"
  ;

  // opts
  command_option(&program, "-V", "--verbose", "enable verbose output", verbose_opt);
  command_option(&program, "-p", "--path <path>", "set the path", path_opt);
  command_option(&program, "-k", "--key <name>", "key to get", key_opt);
  command_option(&program, "-v", "--value <name>", "value to set", value_opt);

  // parse
  command_parse(&program, argc, argv);

  if (NULL == opts.path) {
    if (NULL == default_path) {
      opts.path = getcwd(tmp, sizeof(tmp));
    } else {
      opts.path = default_path;
    }
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
      printf("%s = '%s'\n", trim(key), trim(val));
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
  } else if (0 == strcmp("clear", cmd)) {

    //
    // $ sphia clear --path ~/db
    //

    sphia = sphia_new(opts.path);

    if (NULL == sphia) {
      sphia_error("Failed to open sophia database");
      sphia_free(sphia);
      exit(1);
    }

    rc = sphia_clear(sphia);

    if (-1 == rc) {
      sphia_error("Failed to clear sophia database");
    }

    printf("Cleared sophia database at path '%s'\n", opts.path);
    sphia_free(sphia);
  } else {

    //
    // $ sphia <command> <args>
    //

    char *cargs = trim(strdup(str_flatten(argv, 2, argc)));
    char cbin[64];
    char cexe[1024];
    char *child = NULL;

    sprintf(cbin, "sphia-%s", cmd);

    child = which(trim((char *)cbin));

    if (NULL == child) {
      sphia_ferror("'%s' not a sphia command.\nSee sphia --help for more information.", cmd);
      exit(1);
    }

    sprintf(cexe, "%s %s", child, cargs);

    if (1 == opts.verbose) {
      printf("absolute command path '%s'\n", cexe);
    }

    rc = system(cexe);

    if (1 == opts.verbose) {
      printf("rc = '%d'\n", rc);
    }

    exit(rc);

  }

  command_free(&program);
  return 0;
}


char *
str_flatten (char *array[], int start, int end) {
  char *str = malloc(sizeof(char) * (start + end));
  int i = start;
  strcat(str, "");
  for (; i < end; ++i) {
    strcat(str, array[i]);
    strcat(str, " ");
    if (NULL == array[i]) {
      break;
     }
  }

  return str;
}
