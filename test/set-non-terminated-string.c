
#include <stdlib.h>
#include "sphia-test.h"

static void
test_non_terminated_string() {
  char key[3] = { 'k', 'e', 'y' };
  char value[] = "value";
  assert(-1 != sphia_set(sphia, key, value));

  char *res = sphia_get(sphia, key);
  assert(0 == strcmp(value, res));
}

TEST(test_non_terminated_string);
