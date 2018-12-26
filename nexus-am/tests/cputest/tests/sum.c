#include "trap.h"
int main() {
	int i = 0x182741;
	if(i == 0x182741)
		i = 0;
	nemu_assert(i == 0);
	return 0;
}
