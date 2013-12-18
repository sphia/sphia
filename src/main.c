
//
// main.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <trim/trim.h>
#include <case/case.h>
#include <which/which.h>
#include <str-flatten/str-flatten.h>
#include "file.h"
#include "api.h"
#include "main.h"

static int freePath = 0;
static const char *usage =
  "<command> [--key <key>] [--value <value>] [--path <path>]\n"
  "                       [--config <config>] [--version] [--help]"

  "\n\n"
  "commands:\n"
  "   init                         initialize a new database\n"
  "   get -k <key>                 get a value by key\n"
  "   set -k <key> -v <value>      set a value by key\n"
  "   rm -k <key>                  remove a value by key\n"
  "   list                         list all keys and values\n"
  "   clear                        clears database of all keys\n"
  "   status                       check status of database\n"
  "   purge                        purge database of all corrupt and incomplete data\n"
  "   reset                        reset database of all data\n"
  "   count                        get key count\n"
  "   search [-k <arg>] [-v <arg>] search for a key or value\n";

KEY_OPT(key);
KEY_OPT(value);
KEY_OPT_BOOL(verbose);
KEY_OPT_BOOL(strict);

static void path_opt (command_t *);
static void path_opt (command_t *self) {
  if (1 == freePath) {
    free(opts.path);
    freePath = 0;
  }

  opts.path = (char *)self->arg;
}

static void config_opt (command_t *);
static void config_opt (command_t *self) {
  int rc = read_options(&opts, self->arg, 1);
  if (-1 == rc) {
    sphia_db_error("Failed to read config path %s.", self->arg);
  } else {
    opts.config = (char *)self->arg;
  }

  if (1 == rc) {
    freePath = 1;
  }
}

GEN_CMD(get) {
  // $ sphia get --key foo --path ~db/

  if (NULL == opts.key) {
    sphia_ferror("%s", "Missing '--key <name>' flag");
    return 1;
  }

  opts.key = trim(opts.key);
  char *value = sphia_get(sphia, opts.key);
  if (NULL == value) {
    char *msg = sp_error(sphia->db);
    if (NULL != msg) {
      sphia_ferror("An error occured. %s", msg);
      return 1;
    }

    sphia_ferror("Couldn't find '%s'", opts.key);
    return 0;
  }

  printf("%s\n", value);
  return 0;
}

GEN_CMD(set) {
  // $ sphia set --key foo --value bar --path ~/db

  if (NULL == opts.key) {
    sphia_ferror("%s", "Missing '--key <name>' flag");
    return 1;
  } else if (NULL == opts.value) {
    sphia_ferror("%s", "Missing '--value <value>' flag");
    return 1;
  }

  opts.key = trim(opts.key);
  opts.value = trim(opts.value);
  int rc = sphia_set(sphia, opts.key, opts.value);
  if (-1 == rc) {
    sphia_ferror("An error occured setting key '%s'. %s", opts.key, sp_error(sphia->db));
    return 1;
  }

  printf("%s = '%s'\n", opts.key, opts.value);
  return 0;
}

GEN_CMD(list) {
  // $ sphia list --path ~/db

  SPHIA_DB_FOREACH(key, val, sphia->db) {
    printf("%s = '%s'\n", trim(key), trim(val));
  }

  if (0 != _rc) {
    sphia_db_error("%s\n", sp_error(sphia->db));
    return 1;
  }

  return 0;
}

GEN_CMD(remove) {
  // $ sphia rm --key name --path ~/db

  int rc = sphia_rm(sphia, opts.key);
  if (-1 == rc) {
    if (NULL == opts.key) {
      sphia_ferror("Error removing keys. %s", sp_error(sphia->db));
    } else {
      sphia_ferror("Error removing key '%s'. %s", opts.key, sp_error(sphia->db));
    }

    return 1;
  }

  return 0;
}

GEN_CMD(clear) {
  // $ sphia clear --path ~/db

  int rc = sphia_clear(sphia);
  if (-1 == rc) {
    sphia_error("Failed to clear sophia database");
    return 1;
  }

  printf("Cleared sophia database at path '%s'\n", opts.path);
  return 0;
}

GEN_CMD(status) {
  // $ sphia status --path <path>

  int rc = sphia_status(sphia);
  if (1 == opts.verbose) {
    printf("rc: '%d'\n", rc);
  }

  printf("status: '%s'\n", (-1 == rc? "OK" : "WARN"));
  return 0;
}

GEN_CMD(purge) {
  // $ sphia purge --path <path>

  int rc = sphia_purge(opts.path);
  if (-1 == rc) {
    sphia_error("Failed to purge database");
    return 1;
  }

  sphia = sphia_new(opts.path);
  if (NULL == sphia) {
    sphia_error("Failed to purge database");
    return 1;
  }

  printf("Purged databased\n");
  sphia_free(sphia);
  return 0;
}

GEN_CMD(reset) {
  // $ sphia reset --path <path>

  int rc = sphia_reset(opts.path);
  if (-1 == rc) {
    sphia_error("Failed to reset database");
    return 1;
  }

  sphia = sphia_new(opts.path);
  if (NULL == sphia) {
    sphia_error("Failed to reset database");
    return 1;
  }

  printf("Reset database\n");
  sphia_free(sphia);
  return 0;
}

GEN_CMD(count) {
  // $ sphia count --path <path>

  int count = sphia_count(sphia);
  if (-1 == count) {
    sphia_db_error("%s\n", sp_error(sphia->db));
    return 1;
  }

  printf("%d keys\n", count);
  return 0;
}

GEN_CMD(search) {
  // $ sphia search [--key <key>] [--value <value>]

  if (NULL == opts.key && NULL == opts.value) {
    sphia_error("Missing '--key <key>' or '--value <value' flag to match");
    return 1;
  }

  int count = 0;
  char *key = NULL;
  char *value = NULL;
  char *pstr = NULL;
  char *tk = NULL;
  char *tv = NULL;

  if (NULL != opts.key) {
    key = strdup(opts.key);
    if (NULL == key) {
      sphia_error("Unable to allocate memory");
      return 1;
    }

    key = case_lower(key);
  }

  if (NULL != opts.value) {
    value = strdup(opts.value);
    if (NULL == value) {
      sphia_error("Unable to allocate memory");
      free(key);
      return 1;
    }

    if (0 == opts.strict) {
      value = case_lower(value);
    }
  }

  SPHIA_DB_FOREACH(k, v, sphia->db) {
    int match = 0;
    tk = strdup(k);
    if (NULL == tk) {
      sphia_error("Unable to allocate memory");
      free(key);
      free(value);
      return 1;
    }
    tv = strdup(v);
    if (NULL == tv) {
      sphia_error("Unable to allocate memory");
      free(tk);
      free(key);
      free(value);
      return 1;
    }
    tk = trim(tk);
    tv = trim(tv);

    if (0 == opts.strict) {
      tk = case_lower(tk);
      tv = case_lower(tv);
    }

    if (NULL != key && NULL != (pstr = strstr(tk, key))) {
      match = 1;
      count++;
    } else if (NULL != value && NULL != (pstr = strstr(tv, value))) {
      match = 1;
      count++;
    }

    if (1 == match) {
      printf("%s = %s\n", trim(k), trim(v));
    }

    free(tk);
    free(tv);
  }

  if (0 != _rc) {
    sphia_db_error("%s\n", sp_error(sphia->db));
    free(key);
    free(value);
    return 1;
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

  free(key);
  free(value);
  return 0;
}

static cmd_t cmds[] = {
  {"init", NULL, 1, NULL},
  {"get", NULL, 1, cmd_get},
  {"set", NULL, 1, cmd_set},
  {"remove", "rm", 1, cmd_remove},
  {"list", "ls", 1, cmd_list},
  {"clear", NULL, 1, cmd_clear},
  {"status", "st", 1, cmd_status},
  {"purge", NULL, 0, cmd_purge},
  {"reset", NULL, 0, cmd_reset},
  {"count", NULL, 1, cmd_count},
  {"search", NULL, 1, cmd_search},
};

// Parse the given argv with the appropriate options.
static int
parse_opts (command_t *, int, char *[]);
static int
parse_opts (command_t *commander, int argc, char *argv[]) {
  int hasConfig = 0;
  int rc = 0;

  // If a config option is not set, read default config file.
  for (int i = 0; i < argc; i++) {
    if (0 == strcmp("-c", argv[i]) || 0 == strcmp("--config", argv[i])) {
      hasConfig = 1;
      break;
    }
  }
  if (0 == hasConfig) {
    char config_path[256];
    char *default_path = getenv(DEFAULT_OPT_ENV);
    char *cwd = getcwd(config_path, sizeof(config_path));
    if (NULL != cwd) {
      sprintf(config_path, "%s/.%src", config_path, BIN_NAME);
    }

    if (NULL != cwd && 1 == file_exists(config_path)) {
      rc = read_options(&opts, config_path, 0);
      if (-1 == rc) {
        return -1;
      } else {
        opts.config = config_path;
      }

      if (1 == rc) {
        freePath = 1;
      }
    } else if (NULL != default_path) {
      sprintf(config_path, "%s/.%src", default_path, BIN_NAME);
      rc = read_options(&opts, config_path, 0);
      if (-1 == rc) {
        return -1;
      } else {
        opts.config = config_path;
      }

      if (1 == rc) {
        freePath = 1;
      }
    }
  }

  command_init(commander, "sphia", SPHIA_VERSION);

  commander->usage = usage;
  command_option(commander, "-c", "--config <config>", "read a config file", config_opt);
  command_option(commander, "", "--verbose", "enable verbose output", verbose_opt);
  command_option(commander, "-p", "--path <path>", "set the path", path_opt);
  command_option(commander, "-k", "--key <name>", "key to get", key_opt);
  command_option(commander, "-v", "--value <name>", "value to set", value_opt);
  command_option(commander, "-s", "--strict", "strict mode for a command", strict_opt);

  command_parse(commander, argc, argv);
  return 0;
}

int
main (int argc, char *argv[]) {
  char tmp[256];
  char *default_path = getenv("SPHIA_PATH");
  char *cmd = NULL;
  int rc = 0;
  sphia_t *sphia = NULL;
  command_t program;

  rc = parse_opts(&program, argc, argv);
  if (-1 == rc) {
    sphia_error("Couldn't parse options");
    return 1;
  }

  // Display help if no cmd
  if (program.argc <= 0 || strlen(program.argv[0]) <= 0) {
    command_help(&program);
  } else {
    cmd = program.argv[0];
  }

  // Use SPHIA_PATH if given, otherwise cwd if no path.
  if (NULL != default_path) {
    if (1 == freePath) {
      free(opts.path);
      freePath = 0;
    }

    opts.path = default_path;
  }
  if (NULL == opts.path) {
    opts.path = getcwd(tmp, sizeof(tmp));
    if (NULL == opts.path) {
      sphia_error("Unable to get current directory");
      rc = 1;
      goto cleanup;
    }
  }

  if (1 == opts.verbose) {
    printf("path set to %s\n", opts.path);
  }

  // Loop through commands and run it if found
  for (size_t i = 0; i < sizeof(cmds) / sizeof(cmds[0]); i++) {
    if (0 == strcmp(cmds[i].name, cmd) ||
        (NULL != cmds[i].alt && 0 == strcmp(cmds[i].alt, cmd))) {
      if (1 == cmds[i].open) {
        sphia = sphia_new(opts.path);
        if (NULL == sphia) {
          sphia_ferror("Error initializing sophia database to path '%s'", opts.path);
          rc = 1;
          goto cleanup;
        }
      }

      if (NULL != cmds[i].func) {
        rc = cmds[i].func(sphia);
      }

      if (1 == cmds[i].open) {
        sphia_free(sphia);
      }
      goto cleanup;
    }
  }

  // Unknown command, try sphia-<cmd> in PATH
  char cexe[1024];
  int i = 0;
  int size = 1;
  int found = 0;
  char *loc = NULL;
  char *cargs = NULL;

  for (; i < program.argc; i++) {
    size += strlen(program.argv[i]) + 1;
  }

  cargs = malloc(size);
  if (NULL == cargs) {
    sphia_error("Unable to allocate memory");
    rc = 1;
    goto cleanup;
  }

  // Retrieve arguments separated by spaces
  strcpy(cargs, "\0");
  for (i = 0; i < program.argc; i++) {
    if (0 == strcmp(cmd, program.argv[i]) && 0 == found) {
      found = 1;
      continue;
    }

    strcat(cargs, program.argv[i]);
    strcat(cargs, " ");
  }

  // Get absolute path
  sprintf(cexe, "sphia-%s", cmd);
  loc = which(trim(cexe));
  if (NULL == loc) {
    sphia_ferror("'%s' not a sphia command.\nSee sphia --help for more information.", cmd);
    rc = 1;
    free(cargs);
    goto cleanup;
  }
  if (1 == opts.verbose) {
    printf("absolute command path '%s'\n", cexe);
  }

  sprintf(cexe, "%s %s", loc, cargs);
  rc = system(cexe);
  free(loc);
  free(cargs);
  goto cleanup;

cleanup:
  if (1 == freePath) {
    free(opts.path);
  }
  command_free(&program);
  return rc;
}
