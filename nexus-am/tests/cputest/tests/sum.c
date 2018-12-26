#include "trap.h"
int main() {
	int i = 0, res = 0;
	for(i = 0; i < 10; ++i);
		res += i;
	nemu_assert(res);
	return 0;
}
