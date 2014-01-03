
#include <stdlib.h>
#include "sphia-test.h"

#define EXPECTED_TEST_KEYS 1234

static void
test_count() {
  for (int i = 0; i < EXPECTED_TEST_KEYS; i++) {
    char *key = malloc(16);
    if (NULL == key) {
      fprintf(stderr, "Malloc error\n");
      exit(1);
    }
    sprintf(key, "key%03d", i);
    assert(0 == sphia_set(sphia, key, "hello world"));
    free(key);
  }
  assert(EXPECTED_TEST_KEYS == sphia_count(sphia));
}

TEST(test_count);
