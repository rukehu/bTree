/*****************************************************
 * Last modified  : 2019-03-05 14:43
 * Filename       : public.h
 * Author         : Huruke
 * Email          : huruke@163.com
 * Description    :
 ****************************************************/
#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#include <stdio.h>
#include "../inc/tree.h" 


int queue_append_data(int data);
int queue_get_data(int *data);
void queue_clear(void);
int queue_cpy_buff(int *buff, int len);

void clear_btqueue(void);
int btqueue_isempty(void);
void en_btqueue(treenode_pt node);
int out_btqueue(treenode_pt *node);

#endif /*_PUBLIC_H_*/
