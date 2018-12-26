#include "trap.h"
int main() {
	asm("movl %eax, 0x18247\n\t"	
			"cmpl %eax, 0x182741\n\t");
	return 0;
}
