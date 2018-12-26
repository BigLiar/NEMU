#include "trap.h"
int main() {
	int i = 0;
	i = i + 10;
	nemu_assert(i == 10);
	return 0;
}
