#include "klib.h"
#include <stdarg.h>
#include <stdio.h>
#include <am.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static char fe;
static int width;
int scan_int(const char* fmt, int* f_pos){
	int res = 0;
	while(fmt[*f_pos] <= '9' && fmt[*f_pos] > '0'){
			res = res * 10 + fmt[*f_pos] - '0';
			(*f_pos)++;
	}
	return res;
}

void print_int(char *out, int* pos, int src){
	if(src == 0 && width-- <= 0)
		return;
	print_int(out, pos, src / 10);
	out[(*pos)++] = (char)(src % 10 + '0');
}

void print_str(char *out, int* pos, char* src){
	*pos += strlen(src);
	strcat(out, src);
}

void get_value(const char* fmt, int* f_pos, char* out, int* pos, va_list* p_ap){
	int valint;
	char* valstr;
	if(fmt[*f_pos] == 'd'){	
			valint = va_arg(*p_ap, int);
			if(valint == 0) out[(*pos)++] = '0';
			else print_int(out, pos, valint);
	}
	if(fmt[*f_pos] == 's'){
			valstr = va_arg(*p_ap, char*);
			print_str(out, pos, valstr);
	}
	(*f_pos)++;
}

void get_width(const char* fmt, int* f_pos, char* out, int* pos, va_list* p_ap){
	width = 0;
	if(fmt[*f_pos] <= '9' && fmt[*f_pos] > '0')
			width = scan_int(fmt, f_pos);
	printf("width:%d\n", width);
	get_value(fmt, f_pos, out, pos, p_ap);
}

void get_fill_empty(const char* fmt, int* f_pos, char *out, int* pos, va_list* p_ap){
	fe = ' ';
	if(fmt[*f_pos] == '0'){
		fe = '0';
		(*f_pos)++;
	}
	get_width(fmt, f_pos, out, pos, p_ap);
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
  int i = 0, j = 0;
	while(fmt[j]){
		if(fmt[j] != '%')
			out[i++] = fmt[j++];
		else{
			j++;
			get_fill_empty(fmt, &j, out, &i, &ap);
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
