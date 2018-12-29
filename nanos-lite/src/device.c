#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
	size_t i;
	char *cbuf = (char *)buf;
	for(i = 0; i < len && cbuf[i] != 0; i++)
		_putc(cbuf[i]);
	return i;
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
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
	_Device * vga_dev = _device(_DEV_VIDEO);
	_VideoInfoReg info;
	printf("0x%x, 0x%x\n", info, vga_dev);
  //vga_dev->read(_DEVREG_VIDEO_INFO, &info, sizeof(info));
	//sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", info.width, info.height);
}
