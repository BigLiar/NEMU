#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  printf("0x%x", id_src->val);
 	print_asm_template2(mov);
}

make_EHelper(push) {
	uint32_t cc = decoding.opcode & 0x7;

	id_dest->val = id_dest->width == 2 ? cpu.gpr[cc]._16 : cpu.gpr[cc]._32;
	char reg_name[10] = "%";
	strcat(reg_name, id_dest->width == 2 ? regsw[cc] : regsl[cc]);
	strcpy(id_dest->str, reg_name);
	rtl_push(&id_dest->val);
  print_asm_template1(push);
}

make_EHelper(pop) {
	uint32_t cc = decoding.opcode & 0x7;
	char reg_name[10] = "%";
	strcat(reg_name, id_dest->width == 2 ? regsw[cc] : regsl[cc]);
	strcpy(id_dest->str, reg_name);

	rtl_pop(&cpu.gpr[cc]._32);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
	if(2 == id_src->width)
		cpu.gpr[4]._16 = cpu.gpr[5]._16;
	else if(4 == id_src->width)
		cpu.gpr[4]._32 = cpu.gpr[5]._32;
	else
		assert(0);
	rtl_pop(&cpu.ebp);
  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    t0 = !!(cpu.gpr[0]._16 & 0x8000);
		cpu.gpr[2]._16 = t0 ? 0xffff : 0;
  }
  else {
    t0 = !!(cpu.gpr[0]._32 & 0x80000000);
		cpu.gpr[2]._32 = t0 ? 0xffffffff : 0;
  }

	printf("cltd-EAX:%d, ctld-EDX:0x%x\n", t0, cpu.edx);

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);

  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
