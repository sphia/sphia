
#include "repl.h"
#include "eval.h"


int
repl_eval (repl_session_t *sess) {
  char *buf = sess->eval(sess, buffer_string(sess->data.buffer));
  sess->print(sess, buf);
  return sess->rc;
}

