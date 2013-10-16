
//
// main.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "case.h"
#include "which.h"
#include "sphia.h"
#include "commander.h"


// global options
struct options {
  char *path;
  char *key;
  char *value;
  int verbose;
  int strict;
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
KEY_OPT_TRUE(strict);


char *
str_flatten (char *array[], int start, int end);


int
main (int argc, char *argv[]) {
  char *cmd = NULL;

  // ensure cmd is not `NULL`
  if (NULL == argv[1]) {
    cmd = "";
  } else {
    cmd = strdup(argv[1]);
  }

  command_t program;

  char tmp[256];
  char *default_path = getenv("SPHIA_PATH");
  int rc = 0;
  sphia_t *sphia;


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
    "   rm -k <key>                  remove a value by key\n"
    "   ls                           list all keys and values\n"
    "   clear                        clears database of all keys\n"
    "   st                           check status of database\n"
    "   purge                        purge database of all corrupt and incomplete data\n"
    "   reset                        reset database of all data\n"
    "   count                        get key count\n"
    "   search [-k <arg>] [-v <arg>] search for a key or value\n"
  ;

  // opts
  command_option(&program, "-V", "--verbose", "enable verbose output", verbose_opt);
  command_option(&program, "-p", "--path <path>", "set the path", path_opt);
  command_option(&program, "-k", "--key <name>", "key to get", key_opt);
  command_option(&program, "-v", "--value <name>", "value to set", value_opt);
  command_option(&program, "-s", "--strict", "strict mode for a command", strict_opt);

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
      command_free(&program);
      exit(1);
    }

    sphia_free(sphia);

  } else if (0 == strcmp("get", cmd)) {

    //
    // $ sphia get --key foo --path ~/db
    //

    if (NULL == opts.key) {
      sphia_ferror("%s", "Missing '--key <name>' flag");
      command_free(&program);
      exit(1);
    }

    sphia = sphia_new(opts.path);
    char *value = sphia_get(sphia, opts.key);

    if (NULL == value) {
      char *msg = sp_error(sphia->db);

      if (NULL != msg) {
        sphia_ferror("An error occured. %s", msg);
        sphia_free(sphia);
        command_free(&program);
        exit(1);
      } else {
        sphia_ferror("Couldn't find '%s'", opts.key);
        sphia_free(sphia);
        command_free(&program);
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
      command_free(&program);
      exit(1);
    } else if (NULL == opts.value) {
      sphia_ferror("%s", "Missing '--value <value>' flag");
      command_free(&program);
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
      command_free(&program);
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
      command_free(&program);
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
      command_free(&program);
      exit(1);
    }

    rc = sphia_clear(sphia);

    if (-1 == rc) {
      sphia_error("Failed to clear sophia database");
      sphia_free(sphia);
      command_free(&program);
      exit(1);
    }

    printf("Cleared sophia database at path '%s'\n", opts.path);
    sphia_free(sphia);

  } else if (0 == strcmp("st", cmd) || 0 == strcmp("status", cmd)) {

    //
    // $ sphia status --path <path>
    //

    sphia = sphia_new(opts.path);

    if (NULL == sphia) {
      sphia_error("Try 'sphia purge'");
      command_free(&program);
      exit(1);
    }

    rc = sphia_status(sphia);

    if (1 == opts.verbose) {
      printf("rc: '%d'\n", rc);
    }

    printf("status: '%s'\n", (-1 == rc? "OK" : "WARN"));

    sphia_free(sphia);

  } else if (0 == strcmp("purge", cmd)) {

    //
    // $ sphia purge --path <path>
    //

    rc = sphia_purge(opts.path);

    if (-1 == rc) {
      sphia_error("Failed to purge database");
      command_free(&program);
      exit(1);
    }

    sphia = sphia_new(opts.path);

    if (NULL == sphia) {
      sphia_error("Failed to purge database");
      command_free(&program);
      exit(1);
    }

    sphia_free(sphia);

    printf("Purged databased\n");

  } else if (0 == strcmp("reset", cmd)) {

    //
    // $ sphia reset --path <path>
    //

    rc = sphia_reset(opts.path);

    if (-1 == rc) {
      sphia_error("Failed to reset database");
      command_free(&program);
      exit(1);
    }

    sphia = sphia_new(opts.path);

    if (NULL == sphia) {
      sphia_error("Failed to open database");
      command_free(&program);
      exit(1);
    }

    printf("Reset database\n");

    sphia_free(sphia);

  } else if (0 == strcmp("count", cmd)) {

    //
    // $ sphia count --path <path>
    //

    sphia = sphia_new(opts.path);

    if (NULL == sphia) {
      sphia_error("Failed to open database");
      command_free(&program);
      exit(1);
    }

    int count = sphia_count(sphia);

    if (-1 == count) {
      sphia_db_error("%s\n", sp_error(sphia->db));
      command_free(&program);
      sphia_free(sphia);
      exit(1);
    }

    printf("%d keys\n", count);
    sphia_free(sphia);

  } else if (0 == strcmp("search", cmd)) {

    //
    // $ sphia search [--key <key>] [--value <value>]
    //

    sphia = sphia_new(opts.path);

    if (NULL == sphia) {
      sphia_error("Failed to open database");
      command_free(&program);
      exit(1);
    }

    if (NULL == opts.key && NULL == opts.value) {
      sphia_error("Missing '--key <key>' or '--value <value' flag to match");
      command_free(&program);
      sphia_free(sphia);
      exit(1);
    }

    int count = 0;
    int ksize = 0;
    int vsize = 0;
    char *key = NULL;
    char *value = NULL;
    char *pstr = NULL;

    if (NULL != opts.key) {
      key = strdup(opts.key);
      ksize = strlen(opts.key);

      if (NULL != opts.key) {
        key = case_lower(key);
      }
    }

    if (NULL != opts.value) {
      value = strdup(opts.value);
      vsize = strlen(opts.value);

      if (1 != opts.strict) {
        value = case_lower(value);
      }
    }

    SPHIA_DB_FOREACH(k, v, sphia->db) {
      char *tk = trim(strdup(k));
      char *tv = trim(strdup(v));
      int match = 0;

      if (1 != opts.strict) {
        tk = case_lower(tk);
        tv = case_lower(tv);
      }

      // printf("%s - %s\n", tk, tv);

      if (NULL != key &&
          NULL != (pstr = strstr(tk, key))) {

        match = 1;
        count++;

      } else if (NULL != value &&
                 NULL != (pstr = strstr(tv, value))) {

        match = 1;
        count++;
      }

      if (1 == match) {
        printf("%s = %s\n", trim(k), trim(v));
      }
    }

    if (0 == count) {
      if (NULL != opts.key) {
        if (NULL == opts.value) {
          printf("No search results for key '%s'\n", opts.key);
        } else {
          printf("No search results for key '%s' and value '%s'\n", opts.key, opts.value);
        }
      } else if (NULL != opts.value) {
        printf("No search results for value '%s'\n", opts.value);
      }
    }


    sphia_free(sphia);

  } else if (strlen(cmd) > 0) {

    //
    // $ sphia <command> <args>
    //

    char *cargs = trim((char *)strdup(str_flatten(argv, 2, argc)));
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

    command_free(&program);
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
