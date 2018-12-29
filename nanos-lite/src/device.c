#include "common.h"
#include <amdev.h>

static int W, H;

size_t serial_write(const void *buf, size_t offset, size_t len) {
	size_t i;
	char *cbuf = (char *)buf;
	for(i = 0; i < len && cbuf[i] != 0; i++)
		_putc(cbuf[i]);
	return i;
}

static _Device *getdev(uint32_t id) {
  int n;
	for (n = 1; ; n ++) {
    _Device *cur = _device(n);
    if (cur->id == id)
      return cur;
    if (!cur) break;
  }
  assert(0);
  return NULL;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  return 0;
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  assert(offset + len <= sizeof(dispinfo));
  memcpy(buf, dispinfo + offset, len);
  return len;
}



size_t fb_write(const void *buf, size_t offset, size_t len) {
	_Device *dev = getdev(_DEV_VIDEO);
	_FBCtlReg ctl;
  ctl.pixels = (uint32_t *)buf;
 	ctl.x = offset % W;
 	ctl.y	= offset / W;
	ctl.w = 1;
	ctl.h = 1;
	int i;
	for(i = 0; i < len; ++i){
 		if(ctl.y == H - 1 && ctl.x == W - 1)
			return i; 	
		dev->write(_DEVREG_VIDEO_FBCTL, &ctl, sizeof(ctl));
		ctl.x++;
		ctl.y += ctl.x / W;
	 	ctl.x %= W;	
		ctl.pixels++;
	}		
	return i;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
	_Device *vga_dev = getdev(_DEV_VIDEO);
	_VideoInfoReg info;
  vga_dev->read(_DEVREG_VIDEO_INFO, &info, sizeof(info));
	W = info.width;
	H = info.height;
	sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", info.width, info.height);
}

int get_screen_area(){
	return W * H * 4;
}
