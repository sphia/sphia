
#include <stdlib.h>
#include "sphia-test.h"

// See #5
static void
test_similar_keys() {
  char k1[] = "key-1";
  char v1[] = "val-1";
  assert(-1 != sphia_set(sphia, k1, v1));

  char k2[] = "key-10";
  char v2[] = "val-10";
  assert(-1 != sphia_set(sphia, k2, v2));

  char *res = sphia_get(sphia, k1);
  assert(0 == strcmp(v1, res));
}

TEST(test_similar_keys);
