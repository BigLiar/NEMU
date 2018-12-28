#include <unistd.h>
#include <stdio.h>

int main() {
  int res = write(1, "Hello World!\n", 13);
  if(res == 0){
		write(1, "Yes~~~\n", 7);
	}
	int i = 2;
  volatile int j = 0;
  while (1) {
    j ++;
    if (j == 10000) {
      printf("Hello World from Navy-apps for the %dth time!\n", i ++);
      j = 0;
    }
  }
  return 0;
}
