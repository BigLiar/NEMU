#ifndef __COMMON_H__
#define __COMMON_H__

/* Uncomment these macros to enable corresponding functionality. */
#define HAS_CTE
//#define HAS_VME

#define printk printf

#include <am.h>
#include <klib.h>
#include "debug.h"

#ifndef off_t
#define off_t int
#endif

#ifndef mode_t
#define mode_t int
#endif

typedef char bool;
#define true 1
#define false 0
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();
_Context* do_syscall(_Context *c);

size_t fs_write(int fd, void* buf, size_t count);
size_t fs_read(int fd, void* buf, size_t count);
int sys_open(const char *path, int flags, mode_t mode);
int sys_close(int fd);
off_t sys_lseek(int fd, off_t offset, int whence);
size_t fs_filesz(int fd);
size_t serial_write(const void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);


int get_screen_area();
#endif


