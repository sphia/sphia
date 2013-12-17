
#ifndef SPHIA_TEST_H
#define SPHIA_TEST_H 1

#include "../src/api.h"

int __failed_assertions = 0;

/*
 * Simple assertion macro which records
 * count of failed assertions
 */

#define assert(expr) ({ \
  if (!(expr)) { \
    __failed_assertions++; \
    fprintf(stderr \
      , "Assertion error: %s (%s:%d)\n" \
      , #expr \
      , __FILE__ \
      , __LINE__); \
  } \
})

sphia_t *sphia;

#define TEST(func) int main(void) { \
  sphia = sphia_new("./test-db"); \
  assert(sphia != NULL); \
  func(); \
  assert(-1 != sphia_clear(sphia)); \
  sphia_free(sphia); \
  return __failed_assertions; \
}

#endif
