#include "common.h"
#include "syscall.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  switch (a[0]) {
		case 0x1: _yield();
							c->GPRx = 0;
							break;
		case 0x0: _halt(0);
							c->GPRx = 0;
							break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
