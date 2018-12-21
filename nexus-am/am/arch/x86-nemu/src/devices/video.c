#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>
#define W 400
#define H 300
static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
      info->width = W;
      info->height = H;
      return sizeof(_VideoInfoReg);
    }
  }
  return 0;
}

size_t video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _FBCtlReg *ctl = (_FBCtlReg *)buf;

			int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
			int i, j;
      uint32_t *pixels = ctl->pixels;
      for (i = 0; i < h && y + i < H; i++)
				for(j = 0; j < w && x + j < W; j++){
        	memcpy(&fb[(y + i) * W + x + j], pixels, sizeof(uint32_t));
        	pixels++;
				}

			if (ctl->sync) {
        // do nothing, hardware syncs.
      }
      return sizeof(_FBCtlReg);
    }
  }
  return 0;
}

void vga_init() {
}
