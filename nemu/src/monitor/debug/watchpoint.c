#include "monitor/watchpoint.h"
#include "monitor/expr.h"

static WP wp_pool[NR_WP];
static WP *head, *free_;


void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(char* args){
	bool isSuccess;
	WP *wp_ptr = free_;
	if(wp_ptr == NULL)
		assert(0);
	free_ = wp_ptr->next;
	wp_ptr->next = head;
	strcpy(wp_ptr->expr_str, args);
	wp_ptr->old_value = expr(args, &isSuccess);
	assert(isSuccess);
	return head = wp_ptr;
}
bool free_wp(int NO){
	
	WP *wp_ptr = head, *wp = wp_pool+NO;
	if(head == wp){
		head = NULL;
		return true;
	}
	else{
		for(wp_ptr = head; wp_ptr != NULL && wp_ptr->next != wp; wp_ptr = wp_ptr->next);
		if (wp_ptr == NULL) return false;
		wp_ptr->next = wp->next; 
	}
 	wp->next = free_;
	free_ = wp;
	return true;
/*wp_ptr = free;
	if(wp_ptr == NULL || wp_ptr->NO > wp->NO){
		wp->next = free;
		free = wp;
	}
	else{
		for(wp_ptr2 = wp_ptr->next; wp_ptr != NULL && wp_ptr2->NO < wp->NO; wp_ptr = wp_ptr->next, wp_ptr2 = wp_ptr2->next);
		wp->next = wp_ptr2;
		wp_ptr->next = wp;
	}*/
}

bool wp_is_changed(){
		bool isSuccess;
		bool res = false;
		WP *wp_ptr = head;
		for(wp_ptr = head; wp_ptr != NULL; wp_ptr = wp_ptr->next){
			uint32_t new_value= expr(wp_ptr->expr_str, &isSuccess);
			if(new_value != wp_ptr->old_value){
				printf("Program execution has interrupted.\nWatchpoint %d:%s:\n%u -----> %u\n", wp_ptr->NO, wp_ptr->expr_str, wp_ptr->old_value, new_value);

				wp_ptr->old_value = new_value;
				res = true;
			}
		}
		return res;
}

WP* show_wp(int NO){
	assert( 0 <= NO || NO < NR_WP);
	WP *wp = wp_pool+NO, *wp_ptr;
	for(wp_ptr = head; wp_ptr != NULL && wp_ptr != wp; wp_ptr = wp_ptr->next);

	return wp_ptr;
}
