#include "common.h"
#include "syscall.h"


int sys_write(int fd, void* buf, size_t count){
	if(fd == 1 || fd == 2){
		size_t i;
		char *cbuf = (char *)buf;
		for(i = 0; i < count; ++i)
			_putc(cbuf[i]);
		return count;
	}
	return -1;
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;
  switch (a[0]) {
		case SYS_yield: _yield();
							c->GPRx = 0;
							break;
		case SYS_exit: _halt(0);
							c->GPRx = 0;
							break;
		case SYS_write: c->GPRx = sys_write((int) a[1], (void *) a[2], (size_t) a[3]);
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}

