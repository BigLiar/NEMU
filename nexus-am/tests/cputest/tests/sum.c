#include "trap.h"

int main() {
	int i = 1;
	volatile int sum = 0;
	for(i = 1; i <= 100; i++) {
		sum += i;
	}

	nemu_assert(sum == 5050);

	return 0;
}
