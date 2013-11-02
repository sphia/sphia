
#include <stdlib.h>
#include "sphia-test.h"

static void
test_set_get() {
  char key[] = "test-set-key";
  char value[] = "test-set-value";

  int rc = sphia_set(sphia, key, value);
  assert(-1 != rc);

  char *actual = sphia_get(sphia, key);
  assert(0 == strcmp(actual, value));
}

int
main(int argc, char *argv[]) {
  test_setup();
  test_set_get();
  test_teardown();

  return 0;
}
