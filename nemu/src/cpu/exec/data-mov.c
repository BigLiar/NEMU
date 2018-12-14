#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
	printf("0x%x", decoding.opcode);
	switch(decoding.opcode){
		case 0x50:
			rtl_push(&cpu.eax);
			break;
		case 0x51:
			rtl_push(&cpu.ebx);
			break;
		case 0x52:
			rtl_push(&cpu.ecx);
			break;
		case 0x53:
			rtl_push(&cpu.edx);
			break;
		case 0x54:
			rtl_push(&cpu.esp);
			break;
		case 0x55:
			rtl_push(&cpu.ebp);
			break;
		case 0x56:
			rtl_push(&cpu.esi);
			break;
		case 0x57:
			rtl_push(&cpu.edi);
			break;
	}
  print_asm_template1(push);
}

make_EHelper(pop) {
  TODO();

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
