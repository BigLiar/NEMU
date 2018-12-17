#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
	printf("%d, 0x%x\n", id_dest->val, id_dest->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
	uint32_t cc = decoding.opcode & 0xf;
	id_dest->val = id_dest->width == 2 ? cpu.gpr[cc]._16 : cpu.gpr[cc]._32;
	char reg_name[10] = "%";
	strcat(reg_name, id_dest->width == 2 ? regsw[cc] : regsl[cc]);
	strcpy(id_dest->str, reg_name);
	rtl_push(&id_dest->val);
	printf("%d, 0x%x\n", id_dest->val, id_dest->val);
  print_asm_template1(push);
}

make_EHelper(pop) {
	uint32_t cc = decoding.opcode & 0xf;
	rtl_pop(&id_dest->val);
	char reg_name[10] = "%";
	strcat(reg_name, id_dest->width == 2 ? regsw[cc] : regsl[cc]);
	strcpy(id_dest->str, reg_name);
	printf("%d, 0x%x\n", id_dest->val, id_dest->val);
	cpu.gpr[cc]._32 = id_dest->val;

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
  TODO();

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

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
	printf("%d, 0x%x\n", t0, t0);

  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
	printf("%d, 0x%x\n", t0, t0);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
