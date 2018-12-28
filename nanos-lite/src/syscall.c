#include "common.h"
#include "syscall.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
	printf("syscall-number:0x%x\n", a[0]);
  switch (a[0]) {
//		case 0x100: SYS_yield();
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
