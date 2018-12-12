#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"
#define NR_WP 32

typedef struct watchpoint {
  int NO;
  char expr_str[64];	
	uint32_t old_value;
	
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

} WP;

bool wp_is_changed();
WP* new_wp(char* args);
WP* show_wp(int NO);
bool free_wp(int NO);
#endif
