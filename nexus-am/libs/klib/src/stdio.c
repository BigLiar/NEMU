#include "klib.h"
#include <stdarg.h>
#include <am.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void sprintf_int(char *out, int* pos, int src){
	if(src == 0)
		return;
	sprintf_int(out, pos, src / 10);
	out[(*pos)++] = (char) src % 10 + '0';
}

void sprintf_str(char *out, int* pos, char* src){
	*pos += strlen(src);
	strcat(out, src);
}

int printf(const char *fmt, ...) {
	char buf[256] = {0};
	va_list ap;
	va_start(ap, fmt);
	int res = vsprintf(buf, fmt, ap);
	int i = 0;
	for(i = 0; buf[i] != '\0'; i++)
		_putc(buf[i]);
	
	return res;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
   int i = 0;
	int valint;
	char* valstr;
	while(*fmt){
		if(*fmt != '%')
			out[i++] = *(fmt++);
		else{
			fmt++;
			switch(*fmt){
				case 'd':
					valint = va_arg(ap, int);
					if(valint == 0) out[i++] = '0';
					else sprintf_int(out, &i, valint);
					break;
				case 's':
					valstr = va_arg(ap, char*);
					sprintf_str(out, &i, valstr);
					break;
			}
			fmt++;
		}
	}
	out[i] = '\0';
	return i;
}

int sprintf(char *out, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	return vsprintf(out, fmt, ap);
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}


#endif
