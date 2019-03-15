/*****************************************************
 * Last modified : 2019-03-08 17:05
 * Filename       : bt_stack.h
 * Author         : Huruke
 * Email         : huruke@163.com
 * Description    :
 ****************************************************/
#ifndef  __STACK_H__
#define  __STACK_H__
#include <stdio.h> 
#include "../inc/tree.h"

typedef treenode_pt SElemType;

typedef struct stack_s {
    SElemType data;			  /* 数据域*/
    struct stack_s * next;    /* next 域*/
} stack_t, *stack_pt;

extern stack_pt stack_opt;    /* 栈操作句柄*/

void stack_init(void);
stack_pt create_stack(void);             
int is_empty(stack_pt stack);             
void destory_stack(stack_pt stack);      
int push_stack(stack_pt stack, SElemType elem);
int pop_stack(stack_pt stack, SElemType *elem_p);
void stack_clear(stack_pt stack);         
SElemType get_top_element(stack_pt stack);  

#endif /* __STACK_H__ */
