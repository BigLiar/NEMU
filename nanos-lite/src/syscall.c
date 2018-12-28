#include "common.h"
#include "syscall.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
	printf("syscall-number:0x%x\n", a[0]);
  switch (a[0]) {
		case 0x1: _yield();
							c->GPRx = 0;
							break;
		case 0x0: _halt(1);
							c->GPRx = 0;
							break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
