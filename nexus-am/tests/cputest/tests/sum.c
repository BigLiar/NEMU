#include "trap.h"
int main() {
	asm("cmpl %eax, 0x123\n\t");
	return 0;
}
