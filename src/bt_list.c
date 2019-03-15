/*****************************************************
 * Last modified  : 2019-03-06 11:24
 * Filename       : bt_list.c
 * Author         : Huruke
 * Email          : huruke@163.com
 * Description    : 提供二叉树以及相关信息结构的存取操作接口
 ****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "../inc/tree.h"
#include "../inc/bt_list.h"

/* 二叉树结构列表表头 */
static btlist_pt btree_list;


/* binary list init. */
void btlist_init(void)
{
	btree_list = (btlist_pt)malloc(sizeof(btlist_t));
	btree_list->bt_tree = NULL;
	btree_list->bt_next = NULL;
}

/* reset binary tree struct list is empty */
void btlist_reset(void)
{
	btlist_pt p_node;
	btlist_pt d_node;

	if (btree_list->bt_next != NULL) {
		p_node = btree_list->bt_next;

		while (p_node != NULL) {
			d_node = p_node;
			p_node = d_node->bt_next;

			remove_tree(&(d_node->bt_tree));    //remove binary tree
			free(d_node);                       // free the list node
		}
		btree_list->bt_next = NULL;
	}
}

/* append btree list node into list */
int append_btlist_node(btlist_pt p_node)
{
	btlist_pt last_node = btree_list;

	if (p_node != NULL) {
		/*lookup the last node*/
		while (last_node->bt_next != NULL) {
			last_node = last_node->bt_next;
		}
		
		p_node->bt_next = NULL;	
		last_node->bt_next = p_node;

		return 1;    //return success.
	}

	return 0;
}

void get_tree_list(char **p_buff, int *len)
{
	int node_len = 0;
	btlist_pt look_node = btree_list->bt_next;

	while (look_node != NULL) {
		*p_buff = look_node->bt_name;
		node_len++;
		p_buff++;
		look_node = look_node->bt_next;
	}

	*len = node_len; 
}

/*
 * @brief : get a tree by name.
 * @param :
 *     t_name: 二叉树名
 * @return: 
 *		成功:返回树的根节点
 *		错误:返回NULL
 */
treenode_pt get_btlist_btree(const char *t_name)
{
	btlist_pt look_node = btree_list->bt_next;

	while (look_node != NULL) {
		// 查找名字对应的树
		if (strcasecmp(look_node->bt_name, t_name) == 0) {
			return look_node->bt_tree;
		}
		look_node = look_node->bt_next;
	}
	

	return NULL;    //@TODO:the tree is not found
}


/* remove one node form btree_list. */
 int remove_btlist_btree(const char *t_name)
{
	btlist_pt look_node = btree_list;
	btlist_pt d_node;

	while (look_node->bt_next != NULL) {
		d_node = look_node->bt_next;
		// 查找名字对应的树
		if (strcasecmp(d_node->bt_name, t_name) == 0) {
			look_node->bt_next = d_node->bt_next;    // 断开待删除的节点
			remove_tree(&(d_node->bt_tree));         // remove binary tree
			free(d_node);                            // free the list node
			
			return 1;
		}
		
		look_node = look_node->bt_next;    // 指向下一个节点
	}

	return 0;    //@todo:the tree is not found
}

int get_list_btheight(const char *t_name)
{
	btlist_pt h_node = btree_list->bt_next;
	int height = 0;

	while (h_node != NULL) {
		// 查找名字对应的树
		if (strcasecmp(h_node->bt_name, t_name) == 0) {
			height = get_tree_height(h_node->bt_tree);
			break;
		}
		h_node = h_node->bt_next;
	}

	return height;
}

int clear_list_btree(const char *t_name)
{
	btlist_pt look_node = btree_list;
	btlist_pt l_node;

	while (look_node->bt_next != NULL) {
		l_node = look_node->bt_next;
		// 查找名字对应的树
		if (strcasecmp(l_node->bt_name, t_name) == 0) {
			clear_tree(l_node->bt_tree);
			return 1;
		}
	}	

	return 0;
}



