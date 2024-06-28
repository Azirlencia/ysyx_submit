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

#include "sdb.h"
#include "watchpoint.h"
#define NR_WP 32
/*
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  //TODO: Add more members if necessary 
  bool flag;
  char exp[100];
  int new_;
  int pre_;

} WP;
*/

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(){
  if(free_ == NULL){
    printf("Invalid watch point.\n");
    assert(0);
  }
  for(WP* p = free_ ; p -> next != NULL ; p = p -> next){
    if( p -> flag == false){
      p -> flag = true;
      if(head == NULL){
        head = p;
      }
      return p;
    }
  }
    printf("Full watchpoint\n");
    assert(0);
    return NULL;

}

void free_wp(WP* wp){
  if (wp == NULL) {
    printf("Attempt to free a null watchpoint.\n");
    assert(0);
  }
  if(head -> NO == wp -> NO){
    head -> flag = false;
    head = NULL;
    printf("Delete watchpoint  success.\n");
    return ;
    }
  for(WP* p = head ; p -> next != NULL ; p = p -> next){
    if(p -> next -> NO  == wp -> NO){
        p -> next = p -> next -> next;
        p -> next -> flag = false; 
        printf("free succes.\n");
        return ;
    }
  }
}


void print_watchpoint(){
  bool flag = true;
    for(int i = 0 ; i < NR_WP ; i ++){
      if(wp_pool[i].flag){
        printf("The express of No.%d Watchpoint is \"%s\", old value = %d, new value = %d\n",  \
        wp_pool[i].NO, wp_pool[i].exp,wp_pool[i].pre_, wp_pool[i].new_);
        flag = false;
      }
      if(flag){
      printf("No watchpoint now.\n");
      }
    }
}

void delete_watchpoint(int a){
  for(int i = 0 ; i < NR_WP ; i ++){
    if(wp_pool[i].NO == a){
      free_wp(&wp_pool[i]);
      printf("Delete watchpoint  success.\n");
      return ;
    }
    
  }
}    

void creat_watchpoint(char *args){
  WP* wp = new_wp();
  strcpy(wp -> exp, args);
  bool success = false;
  int val = expr(wp -> exp,&success);
  if(success){
    wp -> pre_ = val;
  }
  else{
    printf("A failure happens at expression evaluation.\n");
  }
  printf("Create watchpoint No.%d success.\n", wp -> NO);
}



