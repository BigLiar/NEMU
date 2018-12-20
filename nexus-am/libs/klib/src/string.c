#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
	while(*(s++))
		len++;
	return len;
}

char *strcpy(char* dst,const char* src) {
  size_t i;
	for(i = 0; src[i] != '\0'; i++)
			dst[i] = src[i];
	dst[i] = '\0';
	return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  size_t i;
	for(i = 0; i < n && src[i] != '\0'; i++)
			dst[i] = src[i];
	for(; i < n; ++i)
			dst[i] = '\0';
	return dst;
}

char* strcat(char* dst, const char* src) {
  size_t len = strlen(dst);
	size_t i;
	for(i = 0; src[i] != '\0'; i++)
			dst[len + i] = src[i];
	dst[len + i] = '\0';
	return dst;
}

int strcmp(const char* s1, const char* s2) {
  size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	size_t len = len1 > len2 ? len1 : len2;
	size_t i;
	for(i = 0; i < len; ++i){
		if(s1[i] > s2[i]) return 1;
		if(s1[i] < s2[i]) return -1;
	}
	return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	size_t len = len1 > len2 ? len1 : len2;
	size_t i;
	for(i = 0; i < n && i < len; ++i){
		if(s1[i] > s2[i]) return 1;
		if(s1[i] < s2[i]) return -1;
	}
	return 0;
}

void* memset(void* v,int c,size_t n) {
  size_t i;
	char* cv = (char*)v;
	for(i = 0; i < n; ++i)
		cv[i] = c && 0xff;
	return v;
}

void* memcpy(void* out, const void* in, size_t n) {
	assert(!(out < in && out + n > in));
	assert(!(in < out && in + n > out));
  size_t i;
	char* cout = (char*)out;
	char* cin = (char*)in;
	for(i = 0; i < n; ++i)
		cout[i] = cin[i];
	return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  unsigned char* ucs1 = (unsigned char*)s1;
	unsigned char* ucs2 = (unsigned char*)s2;
	size_t i;
	for(i = 0; i < n; ++i){
		if(ucs1[i] > ucs2[i]) return 1;
		if(ucs1[i] < ucs2[i]) return -1;
	}
	return 0;
}

#endif
