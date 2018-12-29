#include <assert.h>
#include <stdlib.h>
#include <ndl.h>
#include <stdio.h>
int main() {
	printf("%x\n", malloc(5)); 
	NDL_Bitmap *bmp = (NDL_Bitmap*)malloc(sizeof(NDL_Bitmap));
  int i = NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
  assert(i != -1);
	assert(bmp->pixels);
  NDL_OpenDisplay(bmp->w, bmp->h);
  NDL_DrawRect(bmp->pixels, 0, 0, bmp->w, bmp->h);
  NDL_Render();
  NDL_CloseDisplay();
  while (1);
  return 0;
}
