/*****************************************************
 * Last modified : 2019-03-06 11:16
 * Filename      : bt_list.h
 * Author        : Huruke
 * Email         : huruke@163.com
 * Description   :
 ****************************************************/
#ifndef _BT_LIST_
#define _BT_LIST_

#include "tree.h"

#define NAME_MAX        (64L)        //tree name max len

/* define a two-awy list */
typedef struct btlist_s {
	char bt_name[NAME_MAX];          /* 二叉树名 */
	treenode_pt bt_tree;             /* 二叉树根节点 */
	struct btlist_s *bt_next;        /* next域 */
} btlist_t, *btlist_pt;



void btlist_init(void);
void btlist_reset(void);
void get_tree_list(char **p_buff, int *len);
int append_btlist_node(btlist_pt p_node);
treenode_pt get_btlist_btree(const char *t_name);
int clear_list_btree(const char *t_name);
int remove_btlist_btree(const char *t_name);
int get_list_btheight(const char *t_name);


#endif /* _BT_LIST_ */
