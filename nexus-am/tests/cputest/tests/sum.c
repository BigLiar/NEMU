#include "trap.h"
#include <stdio.h>
int main() {
	int i = 0x182741;
	if(i == 0x182741)
		printf("no error!");
	printf("ox%x", i);
	return 0;
}
