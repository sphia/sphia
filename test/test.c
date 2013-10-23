
#include <stdlib.h>
#include <stdio.h>
#include "../src/sphia.h"

// global sphia pointer
sphia_t *sphia;

static void
test_setup() {
  sphia = sphia_new("./test-db");
  assert(sphia);
}

static void
test_set_get() {
  char key[] = "test-set-key";
  char value[] = "test-set-value";

  int rc = sphia_set(sphia, key, value);
  assert(-1 != rc);

  char *actual = sphia_get(sphia, key);
  assert(0 == strcmp(actual, value));
}

static void
test_teardown() {
  sphia_free(sphia);
}

int
main(int argc, char *argv[]) {
  test_setup();
  test_set_get();
  test_teardown();

  return 0;
}
