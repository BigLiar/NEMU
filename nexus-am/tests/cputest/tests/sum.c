#include "trap.h"
int main() {
	asm("cmp %eax, 182741\n\t");
	return 0;
}
