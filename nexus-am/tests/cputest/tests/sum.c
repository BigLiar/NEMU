#include "trap.h"
#include <stdio.h>
int main() {
	int i = 1;
	volatile int sum = 0;
	if(i && 320 == 320){
		printf("hello!");
	}

	nemu_assert(sum == 5050);

	return 0;
}
