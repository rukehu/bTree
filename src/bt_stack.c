/*****************************************************
 * Last modified : 2019-03-08 09:03
 * Filename      : stack.c
 * Author        : Huruke
 * Email         : huruke@163.com
 * Description   : 提供栈操作功能相关接口
 ****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "../inc/bt_stack.h"

stack_pt stack_opt;        /* 栈操作句柄*/

void stack_init(void)
{
	while (NULL == (stack_opt = create_stack())) {
		sleep(10);
		printf("stack init error.\n");
	}
}

/* 创建一个空栈*/
stack_pt create_stack(void)
{
    stack_pt stack = (stack_pt)malloc(sizeof(stack_t));

    if (NULL != stack) {
       stack->next = NULL;

       return stack;
    }

    return NULL;
}


/* 清空栈*/
void stack_clear(stack_pt stack)
{
	SElemType elem;

    while (!is_empty(stack)) {
		pop_stack(stack, &elem);   //出栈
    }

}


/* 销毁栈*/
void destory_stack(stack_pt stack)
{
	stack_clear(stack);
    free(stack);
}


int is_empty(stack_pt stack)
{
	if (stack->next == NULL) {
		return 1;
	}

	return 0;
}

 
/* 将数据压栈*/
int push_stack(stack_pt stack, SElemType elem)
{
    stack_pt newst = (stack_pt)malloc(sizeof(stack_t));

    if (NULL != newst) {
        newst->data = elem;
        newst->next = stack->next;
        stack->next = newst;

        return 1;
    }

    return 0;
}


/*
 * @brief : 将元素出栈
 * @param :
 *		stack :栈的句柄
 *		elem_p:出栈的元素指针
 * @return:
 *		1 : 元素出栈成功
 *		0 : 栈为空
 *
 */
int pop_stack(stack_pt stack, SElemType *elem_p)
{
    stack_pt tmpst;

    if (!is_empty(stack)) {

        tmpst = stack->next;
        stack->next = tmpst->next;
		*elem_p = tmpst->data;

        free(tmpst);

        return 1;
    }

    return 0;
}


/* 获取栈顶元素*/
SElemType get_top_element(stack_pt stack)
{
    if (!is_empty(stack)) {

        return stack->next->data;
    }

    return NULL;
}


void stack_test(void)
{
	stack_pt stack = create_stack();
	//@TODO:测试用例参考tree.c tree_test函数
}


