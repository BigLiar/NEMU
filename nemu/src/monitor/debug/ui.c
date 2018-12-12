#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
		if(args == NULL)
		  cpu_exec(1);
  	else{
  		int n = atoi(args);
  	  cpu_exec(n);
  	}
	return 0;
}

static int cmd_info(char *args){
	int i = 0;
	if(strcmp(args, "r") == 0){
		for(i = 0; i < 8; ++i){
			printf("\t%s = %10d\t( 0x%08X ) \n", regsl[i], cpu.gpr[i]._32, cpu.gpr[i]._32);
		}
	}
	else if(strcmp(args, "w") == 0){
			for(i = 0; i < NR_WP; ++i){
					WP* wp = show_wp(i);
					if(wp == NULL) continue;
					printf("%d:\t%s = %10d\t( 0x%08x )\n", i, wp->expr_str, wp->old_value, wp->old_value);	
			}
	}
	else assert(0);
	return 0;
}

static int cmd_p(char *args){
	bool success = false;
	int res = expr(args, &success);
	printf("\tresult:%10d\t( 0x%08x )\n",res, res);
	return 0;
}

static int cmd_w(char * args){
	WP *wp = new_wp(args);
	printf("Set watchpoint seccussfully!\n%s = %10d\t(0x%08x)\n", args, wp->old_value, wp->old_value);
	return 0;
}

static int cmd_d(char * args){
	bool ret = free_wp(atoi(args));
	if(ret) printf("Delete watchpoint %s seccussfully!\n", args);
	else printf("Invalid number! Please input againh!\n");
	return 0;
}

static int cmd_x(char * args){
  char *arg = strtok(NULL, " ");
  int n = atoi(arg), i = 0;
	bool isSuccess = false;
  arg = strtok(NULL, " ");
	for(; i < n; ++i)
		vaddr_read(expr(arg, &isSuccess) + i*4, 4);
	return 0;
}

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
	{ "si", "Execute next i program line (after stopping)", cmd_si},
	{ "info", "Display the values of registers or watchpoints", cmd_info},
	{ "p", " Display the value of an expression", cmd_p},
 	{ "w", " Set a breakpoint of the value of an expression ", cmd_w},
 	{ "d", " Delete a breakpoint", cmd_d},
	{ "x", " Display the values of memory", cmd_x}
/* TODO: Add more commands */
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }
  char *str = NULL;
  for (str = NULL; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
