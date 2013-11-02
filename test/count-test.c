
#include <stdlib.h>
#include "sphia-test.h"


static void
test_count() {
  for (int i = 0; i < 1000; i++) {
    char key[16];
    sprintf(key, "key-%03d", i);
    char value[16];
    sprintf(value, "value-%03d", i);
    assert(-1 != sphia_set(sphia, key, value));
  }

  assert(1000 == sphia_count(sphia));

  sphia_clear(sphia);
  assert(0 == sphia_count(sphia));
}

int
main(int argc, char *argv[]) {
  test_setup();
  test_count();
  test_teardown();

  return 0;
}
