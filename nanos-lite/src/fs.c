#include "fs.h"
typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
	size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
	int i;
	for(i = 0; i < NR_FILES; ++i){
		Finfo* finfo_p = file_table + i;
		if(strcmp(finfo_p->name, "/dev/fb") == 0){
			file_table[i].read = invalid_read;
			file_table[i].write = fb_write;	
			file_table[i].size = get_screen_area();
		}
		if(strcmp(finfo_p->name, "/proc/dispinfo") == 0){
			file_table[i].read = dispinfo_read;
			file_table[i].write = invalid_write;
			file_table[i].size = 128;	
		}
	}
}
size_t fs_filesz(int fd){
	return file_table[fd].size;
}

size_t sys_write(int fd, void* buf, size_t count){
		assert(NR_FILES > fd);
		Finfo* finfo_p = file_table + fd; 
		assert(finfo_p->open_offset <= finfo_p->size);
		if(finfo_p->open_offset + count > finfo_p->size)
			count = finfo_p->size - finfo_p->open_offset;
		off_t offset= finfo_p->disk_offset + finfo_p->open_offset;
		return ramdisk_write(buf, offset, count);	
}

size_t sys_read(int fd, void* buf, size_t count){
		assert(NR_FILES > fd);
		Finfo* finfo_p = file_table + fd; 
		assert(finfo_p->open_offset <= finfo_p->size);
		if(finfo_p->open_offset + count > finfo_p->size)
			count = finfo_p->size - finfo_p->open_offset;
		off_t offset= finfo_p->disk_offset + finfo_p->open_offset;
		return ramdisk_read(buf, offset, count);
}

int sys_open(const char *path, int flags, mode_t mode){
	int i = 0;
	Log("open:fd:%d, path:%s\n", i, path);
	for(i = 0; i < NR_FILES; ++ i){
		if(strcmp(file_table[i].name, path) == 0){
			return i;
		}	
	}
	assert(0);
}


int sys_close(int fd) {
	return 0;
}


off_t sys_lseek(int fd, off_t offset, int whence) {
		assert(NR_FILES > fd);
		off_t offset_T = 0;
		if(whence == SEEK_SET)
			offset_T = offset;
		else if(whence == SEEK_CUR)
			offset_T = file_table[fd].open_offset + offset; 
		else if(whence == SEEK_END)
			offset_T = file_table[fd].size + offset;
		assert(offset_T <= file_table[fd].size);
		return file_table[fd].open_offset = offset_T;
}

size_t fs_write(int fd, void* buf, size_t count){
	//Log("fs_write:fd:%d\n", fd);
	size_t ret = 0;
	assert(NR_FILES > fd);
	if(file_table[fd].write == NULL)
		ret = sys_write(fd, buf, count);
	else
	  ret = (*file_table[fd].write)(buf, file_table[fd].open_offset, count);
	file_table[fd].open_offset += ret;
	return ret;
}

size_t fs_read(int fd, void* buf, size_t count){
	assert(NR_FILES > fd);
	size_t ret = 0;
	if(file_table[fd].read == NULL)
		ret = sys_read(fd, buf, count);
	else
	  ret = (*file_table[fd].read)(buf, file_table[fd].open_offset, count);
	file_table[fd].open_offset += ret;
	return ret;
}
