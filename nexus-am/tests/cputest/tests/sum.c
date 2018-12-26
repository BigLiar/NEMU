#include "trap.h"
int main() {
	int i = 0;
	for(i = 0; i < 10; ++i);
	nemu_assert(i == 9);
	return 0;
}
