
#ifndef __SPHIA_TEST_H__
#define __SPHIA_TEST_H__ 1

#include <assert.h>
#include "../src/sphia.h"

// global sphia pointer
sphia_t *sphia;

static void
test_setup() {
  sphia = sphia_new("./test-db");
  assert(sphia != NULL);
}

static void
test_teardown() {
  // empty the db
  sphia_clear(sphia);

  sphia_free(sphia);
}

#endif
