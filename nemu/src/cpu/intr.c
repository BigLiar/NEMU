#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
	rtl_push((uint32_t *)&cpu.eflags);
	rtl_push(&cpu.cs);
	rtl_push(&ret_addr);	
  printf("0x%08x, 0x%08x\n", cpu.esp, cpu.eip);
	t0 = cpu.IDTR.len & 0xffff;
	t1 = NO * 8;
	assert(interpret_relop(RELOP_LT, t1, t0));	
	t0 = cpu.IDTR.base;
	rtl_add(&t0, &t1, &t0);
	rtl_lm(&t1, &t0, 2);
	t0 += 2;
	rtl_lm(&t2, &t0, 4);
	rtl_msb(&t2, &t2, 4);
	assert(t2);
	t0 += 4;
	rtl_lm(&t2, &t0, 2);
	t2 <<= 16;
	rtl_or(&t1, &t1, &t2);
	rtl_jr(&t1);

}

void dev_raise_intr() {
}
