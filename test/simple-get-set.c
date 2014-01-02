
#include <stdlib.h>
#include "sphia-test.h"

static void
test_set_get() {
  char key[] = "test-set-key";
  char value[] = "test-set-value";

  int rc = sphia_set(sphia, key, value);
  assert(-1 != rc);

  char *actual = sphia_get(sphia, key);
  assert(0 == strncmp(actual, value, strlen(actual)));
}

TEST(test_set_get);
