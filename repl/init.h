
#ifndef __REPL_INIT_H__
#define __REPL_INIT_H__ 1

#include <stdio.h>
#include <stdlib.h>


struct repl_session;


#include "read.h"
#include "eval.h"
#include "print.h"
#include "loop.h"
#include "error.h"


// callbacks

typedef void (*repl_loop_cb)(struct repl_session *sess);
typedef void (*repl_print_cb)(struct repl_session *sess, char *buf);
typedef char *(*repl_eval_cb)(struct repl_session *sess, char *cmd);
typedef void (*repl_error_cb)(struct repl_session *sess, char *error);


typedef struct repl_session {
  char *prompt;
  int rc;
  repl_eval_cb eval;
  repl_error_cb error;
  repl_print_cb print;
  FILE *stdin;
  FILE *stdout;
  FILE *stderr;
  repl_read_data_t data;
} repl_session_t;





typedef struct {
  char *prompt;
  repl_eval_cb eval_cb;
  repl_error_cb error_cb;
  repl_print_cb print_cb;
  FILE *stdin;
  FILE *stdout;
  FILE *stderr;
} repl_session_opts;

repl_session_t *
repl_session_new (repl_session_opts *opts);

int
repl_session_start (repl_session_t *session);

void
repl_session_destroy (repl_session_t *session);

char *
repl_session_error ();

char *
repl_session_set_error (char *err);

#endif
