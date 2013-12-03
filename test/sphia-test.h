
#ifndef __SPHIA_TEST_H__
#define __SPHIA_TEST_H__ 1

#include <assert.h>
#include "../src/sphia.h"

sphia_t *sphia;

#define TEST(func) int main(void) { \
  sphia = sphia_new("./test-db"); \
  assert(sphia != NULL); \
  func(); \
  sphia_clear(sphia); \
  sphia_free(sphia); \
  return 0; \
}

#endif
