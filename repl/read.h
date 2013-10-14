
#ifndef __REPL_READ_H__
#define __REPL_READ_H__ 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"


typedef struct repl_read_data {
  buffer_t *buffer;
} repl_read_data_t;


int
repl_read (struct repl_session *sess);

#endif
