
#include "term.h"
#include "repl.h"


int
repl_read (repl_session_t *sess) {
  int rc = 0;
  repl_read_data_t data;
  char buf[REPL_MAX_BUFFER_LENGTH];

  // reset rc state
  sess->rc = 0;

  term_reset();
  printf("%s ", sess->prompt);
  fgets(buf, REPL_MAX_BUFFER_LENGTH, sess->stdin);

  if (NULL == buf || '\0' == buf[0]) {
    return -1;
  }


  data.buffer = buffer_new_with_string(buf);
  sess->data = data;
  rc = repl_eval(sess);

  if (rc != 0) {
    sess->error(sess, repl_session_error());
  }

  return rc;
}
