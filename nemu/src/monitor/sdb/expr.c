/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <memory/paddr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
  TK_NOTYPE = 256, TK_EQ = 127, 
  AND,OR,
  NUM, HEX, TK_NEQ = 126, REG, DEREF, MINUS

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"\\-", '-'},         // sub
  {"\\*", '*'},         // mul
  {"\\/", '/'},         // div
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},  //not equal
  {"\\(", '('},  //left bracket
  {"\\)", ')'},  //right bracket   
  {"\\|\\|", OR},  // or
  {"\\&\\&", AND},  //and
  {"\\!", '!'},  //not
  {"\\$[a-zA-Z]*[0-9]*", REG},  //register
  {"0x[0-9A-Fa-f]+", HEX},
  {"[0-9]*", NUM}  //number
};

#define NR_REGEX ARRLEN(rules)

#define STACK_SIZE 1024
static regex_t re[NR_REGEX] = {};

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

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

int priority(char op);

uint32_t max(uint32_t a, uint32_t b){
	return (a > b) ? a : b;
}
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
		//Token tmp_token;
        switch (rules[i].token_type) {
          case '*':
          case '-':
            if (nr_token == 0 || tokens[nr_token - 1].type == '(' || priority(tokens[nr_token - 1].type) > 0){
              switch (rules[i].token_type)
              {
              case '*':
                tokens[nr_token].type = DEREF;
                break;
              case '-':
                tokens[nr_token].type = MINUS;
                break;
              }
            }else if (tokens[nr_token - 1].type == ')' 
              || tokens[nr_token - 1].type == NUM || tokens[nr_token - 1].type == HEX
              || tokens[nr_token - 1].type == REG){
              tokens[nr_token].type = rules[i].token_type;
            }else {
              IFDEF(CONFIG_DEBUG, Log("face%#xin front", tokens[i - 1].type));
              assert(0);
            }
            nr_token++;
            break;

          case TK_NOTYPE:
            break;
          case NUM:
          case HEX:
          case REG:
            memcpy(tokens[nr_token].str, e + position - substr_len, (substr_len) * sizeof(char));
            tokens[nr_token].str[substr_len] = '\0'; 
          default: 
            tokens[nr_token].type = rules[i].token_type;
            nr_token++;
            break;
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
			//extension
/*
	OR = 251, AND = 250,
 	REG = 249, NUM = 248, LEQ = 247, MEQ = 246,
*/	
/*	
			case 251: //or
				tokens[nr_token].type = 251;
                strcpy(tokens[nr_token].str, "||");
                nr_token++;
                break;
            case 250: //and
            	tokens[nr_token].type = 250;
                strcpy(tokens[nr_token].str, "&&");
                nr_token++;
                break;
			case 249: //reg
				tmp_token.type = 249;
				strncpy(tokens[nr_token].str, &e[position - substr_len], substr_len);
				nr_token++;
				break;
			case 248: // 248:num
				tmp_token.type = 248;
				strncpy(tokens[nr_token].str, &e[position - substr_len], substr_len);
				nr_token++;
				break;
			case 247: //leq
				tokens[nr_token].type = 247;
                strcpy(tokens[nr_token].str, "<=");
                nr_token++;
                break;
            case 246://meq
            	tokens[nr_token].type = 250;
                strcpy(tokens[nr_token].str, ">=");
                nr_token++;
                break;
                  
        	default:  
        		printf("defult");
        		tokens[nr_token].type = rules[i].token_type;
            	nr_token++;
                break;
*/

/* TODO: Insert codes to evaluate the expression. 
bool check_parentheses(int p, int q)
{
    if(tokens[p].type != '(' || tokens[q].type != ')')
        return false;
    int l = p + 1, r = q - 1;
    while(l < r)
    {
        if(tokens[l].type == '(')
        {
            r--;
        }
        else if(tokens[l].type == ')')
        {
            if(tokens[r].type == '(')
            {
                l++;
                r--;
            }
            else
            {
                return false; 
            }
        }
        else
        {
            l++; // ignore
        }
    }
    return l == r; // charters matched at l = r.
}
*/
bool check_parentheses(int p, int q, int *position){
  char stack[STACK_SIZE];
  *position = -1;
  int top = -1, index = p;
  bool is_parentheses = tokens[p].type == '(';
  while (index <= q){
    if (tokens[index].type == '('){
      stack[++top] = '(';
    }else if (tokens[index].type == ')'){
      if (top < 0 || stack[top] != '('){
        *position = p;
        return false;
      }else {
        top--;
      }
    }
    if (index < q)
      is_parentheses = (top >= 0) && is_parentheses; 
    index++;
  }
  if (top != -1){ 
    *position = p;
    return false;
  }
  return is_parentheses;
}            
        
/*
  return 0;
}
/*/
word_t eval(uint32_t p, uint32_t q, bool *success, int *position) {
 	if (p > q) {
    /* Bad expression */
		printf("Bad expression.");
 		assert(0);
  		return -1;
  	}
  	else if (p == q) {
		uint32_t tmp = 0;
    	switch (tokens[p].type){
    		case HEX:
      			sscanf(tokens[p].str, "%x", &tmp);
      			break;
    
    		case NUM:
      			sscanf(tokens[p].str, "%u", &tmp);
      			break;

    		case REG:
      			if (strcmp(tokens[p].str, "$pc") == 0){
        			tmp = cpu.pc;
        			*success = true;
      			}
				else {
        			tmp = isa_reg_str2val(tokens[p].str, success);
      			}

      			if (!*success){
        			*position = p;
              printf("A failure happens at reg print at %ls.",position);
        			return 0;
					
      			}
     			break;

    		default:
          printf("A failure happens at a single token.");
      		assert(0);
  		}
		return tmp;
	}
	else if (q - p == 1 || check_parentheses(p + 1, q, position) == true){//minus and deref
		switch (tokens[p].type) {
			case MINUS:
				return -eval(p + 1, q, success, position);
			case DEREF:
				return  *((uint32_t *)guest_to_host(eval(p + 1, q, success, position)));
			default:
				assert(0);
				printf("A failure happens at minus and deref.");
		}
	}
 	else if (check_parentheses(p, q, position) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    	return eval(p + 1, q - 1, success, position);
  	}
 	else {
		if (*position != -1){
      		*success = false;
      		return 0;
    	}
    	int op = -1;//op = the position of operator in the token expression;
		int f = -1;
    	for(int i = p ; i <= q ; i++){
    		if(tokens[i].type == '('){
				f++;
			}
			else if(tokens[i].type == ')'){
				f--;
			}
			else if(priority(tokens[i].type) >= 0 && f == -1){
				if(op == -1 || priority(tokens[i].type) <= tokens[op].type){
					op = i; // Find the operator with the lowest priority
				}
			}
    }

    
  uint32_t op_type = tokens[op].type;
  uint32_t val1 = eval(p, op - 1, success, position);
	uint32_t val2 = eval(op + 1, q, success, position);
/*
	{" +", TK_NOTYPE},    // spaces
  	{"\\+", '+'},         // plus
  	{"\\-", '-'},         // sub
  	{"\\*", '*'},         // mul
  	{"\\/", '/'},         // div
  	{"==", TK_EQ},        // equal
  	{"!=", TK_NEQ},  //not equal
  	{"\\(", '('},  //left bracket
  	{"\\)", ')'},  //right bracket   
  	{"\\|\\|", OR},  // or
  	{"\\&\\&", AND},  //and
  	{"\\!", '!'},  //not
  	{"\\$[a-zA-Z]*[0-9]*", REG},  //register
  	{"[0-9]*", NUM}  //number
*/
  switch (op_type) {
    case '+': return val1 + val2;
    case '-': return val1 - val2;
    case '*': return val1 * val2;
    case '/': if  (val2 == 0) {
      success = false;
      printf("Div zero error.");
      return 0;
    }
    return val1 / val2;
		//expansion
		case TK_EQ: return val1 == val2;
    case TK_NEQ: return val1 != val2;
    case OR: return val1 || val2;
    case AND: return val1 && val2;
    default: 
			printf("A failure happens at calculation.");
			assert(0);
				
				
  		}
	}
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  *success = true;
  int position = 0;
  int ans = eval(0, nr_token - 1, success, &position);
  if (!*success){
    printf("some problem happens at position %d\n%s\n%*.s^\n", position, e, position, "");
  }
  return ans;
}

int priority(char op){
	switch (op)
	{
	case '|':
    	return 1;

  	case '&':
    	return 2;

  	case TK_EQ:
		return 3;
  	case TK_NEQ:
    	return 3;

  	case '+':
		return 4;
 	case '-':
    	return 4;
  
  	case '*':
		return 5;
  	case '/':
		return 5;

  	default:
    	return -1;
	}
}














