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
  {"stdout", 0, 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}
size_t fs_filesz(int fd){
	return file_table[fd].size;
}

int sys_write(int fd, void* buf, size_t count){
	if(fd == FD_STDOUT || fd == FD_STDERR){	
		size_t i;
		char *cbuf = (char *)buf;
		for(i = 0; i < count && cbuf[i] != 0; i++)
			_putc(cbuf[i]);
		return i;
	}
	else{
		Log("write-count:%d\n", count);
		assert(NR_FILES > fd);
		Finfo* finfo_p = file_table + fd; 
		assert(finfo_p->open_offset <= finfo_p->size);
		if(finfo_p->open_offset + count <= finfo_p->size);
			count = finfo_p->size - finfo_p->open_offset;
		off_t offset= finfo_p->disk_offset + finfo_p->open_offset;
		off_t ret = ramdisk_write(buf, offset, count);	
		finfo_p->open_offset += ret;
		return ret;
	}
}


int sys_read(int fd, void* buf, size_t count){
		Log("read-count:%d\n", count);
		assert(NR_FILES > fd);
		Finfo* finfo_p = file_table + fd; 
		assert(finfo_p->open_offset <= finfo_p->size);
		if(finfo_p->open_offset + count <= finfo_p->size);
			count = finfo_p->size - finfo_p->open_offset;
		off_t offset= finfo_p->disk_offset + finfo_p->open_offset;
		size_t ret = ramdisk_read(buf, offset, count);
		finfo_p->open_offset += ret;
		return ret;
}

int sys_open(const char *path, int flags, mode_t mode){
	int i = 0;
	for(i = 0; i < NR_FILES; ++ i){
		if(strcmp(file_table[i].name, path) == 0)
			return i;
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
		Log("open_offset:%d\n", offset_T);
		return file_table[fd].open_offset = offset_T;
}
