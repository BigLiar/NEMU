#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_LP, TK_RP, TK_NB, TK_REG

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
	{"\\$[a-zA-A]+", TK_REG},
												// register
	{"0x[0-9a-fA-F]+", TK_NB},
	{"[0-9]+", TK_NB},    // num
	{"\\)", TK_RP},				// right paren
	{"\\(", TK_LP},				// left paren
  {" +", TK_NOTYPE},    // spaces
	{"\\-", '-'},					// sub
  {"\\/", '/'},					// div
	{"\\*", '*'},					// mult
  {"\\+", '+'},         // plus
  {"==", TK_EQ}         // equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
					case TK_NOTYPE:break;
					case TK_NB:
					case TK_REG:
						snprintf(tokens[nr_token].str, 32, "%.*s", substr_len, substr_start);
          default:
						tokens[nr_token].type = rules[i].token_type;
						nr_token++;
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

void get_paren_array(int p, int q, int array[]){
	int i = 0, count = 0;
	for(i = p; i <= q; ++i){
		if(tokens[i].type == TK_LP) count++;
		if(tokens[i].type == TK_RP) count--;
		array[i] = count;
	}
}

bool check_parentheses(int p, int q, int array[]){
	int i = p;
	for(; i < q; ++i)
		if(array[i] <= 0) return false;
	if(array[i] != 0) return false;
	return true;
}

int choose_main_OP(int p, int q, int array[]){
	int position_OP = -1;
	int i = 0;
	for(i = p + 1; i <= q; ++i)
		if(array[i] == 0 && tokens[i-1].type != '+' && tokens[i-1].type != '-' && tokens[i-1].type != '*' && tokens[i-1].type != '/'){
			switch(tokens[i].type){
			case '+':
			case '-':position_OP = i;break;
			case '*':
			case '/':
				if(tokens[position_OP].type != '+' && tokens[position_OP].type != '-')
					position_OP = i;
				break;
			}
		}
	return position_OP;
}

uint32_t get_value_by_reg_name(char reg_name[]){
	int i = 0;
	for(i = 0; i < 8; ++i){
		if(strcmp(regsl[i],reg_name) == 0) return reg_l(i);
		if(strcmp(regsw[i],reg_name) == 0) return reg_w(i);
		if(strcmp(regsb[i],reg_name) == 0) return reg_b(i);
		}
	assert(0);
	return 0;
}
int eval(int p, int q) {
  assert(p <= q);
	int array[32] = {0};
	get_paren_array(p, q, array);
  if (p == q) {
    /* Single token.
 *      * For now this token should be a number.
 *           * Return the value of the number.
 *                */
		if(tokens[p].type == TK_NB)
			return strtol(tokens[p].str, 0, 0);
		if(tokens[p].type == TK_REG)
			return get_value_by_reg_name(tokens[p].str+1); 
  }
  else if (check_parentheses(p, q, array) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
 *      * If that is the case, just throw away the parentheses.
 *           */
  	return eval(p + 1, q - 1);
  }
  else {
    int op = choose_main_OP(p, q, array);
		if(op == -1){
			if(tokens[p].type == '+') return eval(p+1, q);
			if(tokens[p].type == '-') return -eval(p+1, q);
			if(tokens[p].type == '*') return vaddr_read(eval(p+1, q), 4);
			assert(0);
		}

    uint32_t val1 = eval(p, op - 1);
    uint32_t val2 = eval(op + 1, q);

    switch (tokens[op].type) {
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': return val1 / val2;
      default: assert(0);
    }	
  }
	return 0;
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  *success = true;
  return eval(0, nr_token - 1);
}
