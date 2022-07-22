#include <stdlib.h>
#include <string.h>

extern "C" void *pvPortMalloc(size_t xWantedSize);
extern "C" void vPortFree(void *pv);

extern "C" void *malloc(size_t size);
extern "C" void *calloc(size_t num, size_t size);
extern "C" void *realloc(void *ptr, size_t nbytes);
extern "C" void *zalloc(size_t nbytes);
extern "C" void free(void *ptr);

void *malloc(size_t size) { return pvPortMalloc(size); }

void *calloc(size_t num, size_t size) { return pvPortMalloc(num * size); }

void *realloc(void *ptr, size_t nbytes) {
  if (nbytes == 0) {
    free(ptr);
    return nullptr;
  }

  void *p = malloc(nbytes);
  if (p) {
    if (ptr != nullptr) {
      memcpy(p, ptr, nbytes);
      free(ptr);
    }
  }
  return p;
}

void *zalloc(size_t nbytes) { return calloc(1, nbytes); }

void free(void *ptr) { return vPortFree(ptr); }

void *operator new(size_t sz) { return malloc(sz); }

void *operator new[](size_t sz) { return malloc(sz); }

void operator delete(void *p) { free(p); }

void operator delete[](void *p) { free(p); }
