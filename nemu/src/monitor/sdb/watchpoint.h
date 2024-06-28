#ifndef WATCHPOINT_H
#define WATCHPOINT_H

#include "sdb.h" 
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  bool flag;
  char exp[100];
  int new_;
  int pre_;

} WP;

void print_watchpoint();
void delete_watchpoint(int a);
void creat_watchpoint(char *args); 


#endif 
