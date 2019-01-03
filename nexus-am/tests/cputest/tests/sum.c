#include "trap.h"
int main() {
	int i = 0;
	int a = 0, b = 1;
	if(a == 0 || b == 2)
		i = 1;
	else
		i = 2;
	nemu_assert(i == 0);
	return 0;
}
