/*****************************************************
 * Last modified  : 2019-03-05 09:08
 * Filename       : tree.h
 * Author         : Huruke
 * Email          : huruke@163.com
 * Description    :
 ****************************************************/
#ifndef _TREE_H_
#define _TREE_H_
#include <stdio.h>

#define DATA_MAX      (1024L)    //接收数据最大长度

extern char swp_buff[DATA_MAX];
extern int node_buff[DATA_MAX];

/* 遍历二叉树方式*/
typedef enum {
	PRE_TRAVEL = 0,    /* 先序遍历二叉树*/
	MID_TRAVEL,
	POST_TRAVEL
} TRAVEL_TYPE;

/* 查找节点类型*/
typedef enum {
	FIND_P = 0,        /* 查找父节点*/
	FIND_B,            /* 查找兄弟节点*/
} FIND_TYPE;

/* 删除节点类型*/
typedef enum {
	TREE_LEFT = 0,        /* 删除左子树*/
	TREE_RIGHT,            /* 删除右子树*/
} TREE_SUB_TYPE;


typedef struct treenode_s{
	int tn_val;
	struct treenode_s *tn_lchild;
	struct treenode_s *tn_rchild;
} treenode_t, *treenode_pt;


void create_tree(treenode_pt *root);

void pre_order_tree(treenode_pt root);
void mid_order_tree(treenode_pt root);
void post_order_tree(treenode_pt root);
void travel_tree_node(treenode_pt root, TRAVEL_TYPE type, int opt);
int layer_order_tree(treenode_pt root, int *buff);
int layer_node_cnt(treenode_pt root, int *buff);
int insert_node(treenode_pt root, int in_val);
int delete_node(treenode_pt root, int d_val);
int get_tree_height(treenode_pt root);
int change_node_val(treenode_pt root, int o_val, int n_val);
void clear_tree(treenode_pt root);
void remove_tree(treenode_pt *root);
int find_node_val(int *val, treenode_pt root, int find_val, FIND_TYPE type);

#endif /* _TREE_H_ */
