#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  char* expr_str;	
	uint32_t old_value;
	
  struct watchpoint *next;

  /* TODO: Add more members if necessary */


} WP;
bool wp_is_changed();
#endif
