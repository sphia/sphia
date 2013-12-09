
//
// srepl.c
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#include <stdio.h>
#include <repl/repl.h>
#include "srepl.h"

repl_session_opts opts;

static sphia_t *sphia_cur = NULL;

static char *
eval (repl_session_t *sess, char *buf);

static void
print (repl_session_t *sess, char *buf);

static void
error (repl_session_t *sess, char *err);


int
sphia_repl (sphia_t *sphia) {
  int rc = 0;

  opts.prompt = "sphia>";
  opts.eval_cb = eval;
  opts.print_cb = print;
  opts.error_cb = error;

  // initiate
  repl_session_t *sess = repl_session_new(&opts);

  // store current `sphia_t *`
  sphia_cur = sphia;

  // start
  rc = repl_session_start(sess);

  // destroy
  repl_session_destroy(sess);
  return rc;
}

static void
print (repl_session_t *sess, char *buf) {
  fprintf(sess->stdout, "%s\n", buf);
  repl_loop(sess);
}

static void
error (repl_session_t *sess, char *err) {
  fprintf(sess->stderr, "error: '%s'\n", err);
  repl_loop(sess);
}

static char *
eval (repl_session_t *sess, char *buf) {
  if (feof(sess->stdin)) {
    sess->rc = 0;
    return NULL;
  }


  printf("%s\n", buf);

  return NULL;
}
