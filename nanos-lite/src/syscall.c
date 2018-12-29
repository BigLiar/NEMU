#include "common.h"
#include "syscall.h"
extern char _end;
void* program_break = (void *)&_end;

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;
  switch (a[0]) {
		case SYS_yield: 
			_yield();
			c->GPRx = 0;
			break;
		case SYS_exit: 
			_halt(a[1]);
			c->GPRx = 0;
			break;
		case SYS_write: 
			c->GPRx = fs_write((int) a[1], (void *) a[2], (size_t) a[3]);
			break;
		case SYS_open:
			c->GPRx = sys_open((const char *) a[1], (int) a[2], (mode_t) a[3]);
			break;
		case SYS_read:
			c->GPRx = fs_read((int) a[1], (void *) a[2], (size_t) a[3]);
			break;
		case SYS_close:
			c->GPRx = sys_close((int) a[1]);
			break;
		case SYS_lseek:
			c->GPRx = sys_lseek((int) a[1], (off_t) a[2], (int) a[3]);
			break;

		case SYS_brk:
			printf("0x%08x\n", a[1]);
			*(void **)a[1] = program_break;
			program_break += a[2];
			Log("brk:old:0x%08x, add:%x,  new:0x%08x\n", *(void **)a[1], a[2], program_break);
			c->GPRx = 0;
			break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}

