#include "trap.h"
int main() {
	asm("movl $0x18273, %eax\n\t"	
			"add %eax, 0x182741\n\t");
	return 0;
}
