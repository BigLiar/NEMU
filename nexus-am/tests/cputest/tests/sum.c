#include "trap.h"
int main() {
	int i = 0, res = 0;
	for(i = 0; i < 10; ++i);
		res = res + i;
	nemu_assert(res == 45);
	return 0;
}
