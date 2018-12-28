#ifndef __ARCH_H__
#define __ARCH_H__

#include <am.h>

#define PMEM_SIZE (128 * 1024 * 1024)
#define PGSIZE    4096    // Bytes mapped by a page

typedef uint32_t size_t;

struct _Context {
	int not_care;
 	uintptr_t	edi, esi, ebp, esp, ebx, edx, ecx, eax;
	int irq;
	uintptr_t err, eip, cs, eflags;
  struct _Protect *prot;
};

#define GPR1 irq
#define GPR2 edx
#define GPR3 ecx
#define GPR4 ebx
#define GPRx 

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
