#include "trap.h"
int main() {
	int i = 0;
	int a = 0, b = 1;
	if(a || b)
		i = 1;
	else
		i = 2;
	nemu_assert(i == 1);
	return 0;
}
