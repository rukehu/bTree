/*****************************************************
 * Last modified  : 2019-03-05 14:31
 * Filename       : tree.c
 * Author         : Huruke
 * Email          : huruke@163.com
 * Description    : 提供二叉树相关操作接口
 ****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/tree.h"
#include "../inc/bt_queue.h"
#include "../inc/bt_stack.h"

char swp_buff[DATA_MAX];    //数据处理交换缓冲区
int node_buff[DATA_MAX];    //树节点有序排列缓冲区
static int fetch_len = 0;
static int fetch_idx = 0;
static int node_len = 0;


/*
 * @brief : 提取缓存区中用户输入的值，用于创建二叉树时调用
 * @param :
 *		val:值提取指针
 * @return:
 *		0 : 表示此处没有节点
 *		1 : 表示此处存在一个节点
 */
static int fetch_node_val(int *val)
{
	char data[12];        //节点的值在一个int二进制范围内
	int idx = 0;

	if (fetch_idx >= fetch_len) {
		return 0;
	}

	while (1) {
		data[idx] = swp_buff[fetch_idx];
		fetch_idx++;
		
		if (data[idx] == ' ') {  //空格为节点之间的分隔
			data[idx] = '\0';
			while (swp_buff[fetch_idx] == ' ') {
				fetch_idx++;
			}
			break;

		} else if(fetch_idx == fetch_len) {
			data[idx+1] = '\0';
			break;
		}

		idx++;
	}

	if (strlen(data) == 0) {
		return 0;
	}

	*val = atoi(data);

	return 1;
}


static void sort_node_buff(void)
{
	int i, j;
	int swp_val, half;

	for (i = 0; i < node_len; i++) {
		for (j = i+1; j < node_len; j++) {
			if (node_buff[i] > node_buff[j]) {
				swp_val = node_buff[i];
				node_buff[i] = node_buff[j];
				node_buff[j] = swp_val;
			}
		}
	}
}


/* 将数据交换缓冲中的节点排序到节点缓冲区中.*/
static void save_node_val(void)
{
	int swp_val;

	node_len = 0;
	while (0 != fetch_node_val(&swp_val)) {
		node_buff[node_len] = swp_val;
		node_len++;
	}
}


static treenode_pt create_node(int val)
{
	treenode_pt new_node = (treenode_pt)malloc(sizeof(treenode_t));
	
	if (NULL != new_node) {
		new_node->tn_val = val;
		new_node->tn_lchild = NULL;
		new_node->tn_rchild = NULL;
	}

	return new_node;
}


/* 设置一颗排序二叉树*/
treenode_pt set_bstree(int *buff, int len)
{
	treenode_pt node = NULL;
	int half = (len-1) / 2;

	if (len > 0) {
		node = create_node(buff[half]);

		if (half > 0) {
			node->tn_lchild = set_bstree(buff, half);
			node->tn_rchild = set_bstree(&buff[half+1], len - (half+1));

		} else if (half == 0 && len == 2) {
			node->tn_rchild = set_bstree(&buff[1], 1);
		}
	}

	return node;
}


/* create binary tree. */
void create_tree(treenode_pt *root) 
{
	fetch_idx = 0;

	fetch_len = strlen(swp_buff);
	save_node_val();
	sort_node_buff();
	*root = set_bstree(node_buff, node_len);
}

/* Preorder order binary tree. */
void pre_order_tree(treenode_pt root)
{
	if (NULL != root) {
		queue_append_data(root->tn_val);    //将数据存放至队列

		pre_order_tree(root->tn_lchild);
		pre_order_tree(root->tn_rchild);
	}
}

void mid_order_tree(treenode_pt root)
{ 
	if (NULL != root) { 
		mid_order_tree(root->tn_lchild); 
		queue_append_data(root->tn_val);

		mid_order_tree(root->tn_rchild);
	}

}

void post_order_tree(treenode_pt root)
{
	if (NULL != root) {
		post_order_tree(root->tn_lchild);
		post_order_tree(root->tn_rchild);

		queue_append_data(root->tn_val);
	}
}



/*非递归前序遍历*/
void nore_pre_travel(treenode_pt root)
{
	treenode_pt p = root;

	//清空栈
	stack_clear(stack_opt);
	queue_clear();

	while (p != NULL || !is_empty(stack_opt)) {
		if (p != NULL) {
			push_stack(stack_opt, p);
			queue_append_data(p->tn_val);
			p = p->tn_lchild;
			
		} else {
			pop_stack(stack_opt, &p);
			p = p->tn_rchild;
		}
	}
}


/*非递归中序遍历*/
void nore_mid_travel(treenode_pt root)
{
	treenode_pt p = root;

	//清空栈
	stack_clear(stack_opt);
	queue_clear();

	while (p != NULL || !is_empty(stack_opt)) {
		if (p != NULL) {
			push_stack(stack_opt, p);
			p = p->tn_lchild;
			
		} else {
			pop_stack(stack_opt, &p);
			queue_append_data(p->tn_val);
			p = p->tn_rchild;
		}
	}
}


/*非递归后序遍历*/
void nore_post_travel(treenode_pt root)
{
	stack_pt s_swp; 
	treenode_pt p = root;

	s_swp = create_stack();
	//清空栈
	stack_clear(stack_opt);
	queue_clear();

	while (p != NULL || !is_empty(stack_opt)) {
		if (p != NULL) {
			push_stack(stack_opt, p);
			push_stack(s_swp, p);
			p = p->tn_rchild;
			
		} else {
			pop_stack(stack_opt, &p);
			p = p->tn_lchild;
		}
	}
	while (!is_empty(s_swp)) {
		pop_stack(s_swp, &p);
		queue_append_data(p->tn_val);
	}

	destory_stack(s_swp);    //回收零时分配的栈
}


/*
 * @brief : 二叉树节点遍历通用接口.
 * @param :
 *		root:二叉树根节点
 *		type:遍历类型(先,中,后)
 *		opt :遍历方式(0为递归,其他值为非递归)
 * @return: Not.
 */
void travel_tree_node(treenode_pt root, TRAVEL_TYPE type, int opt)
{
	int data;

	if (root == NULL) {
		return;
	}
	queue_clear();
	switch (type) {
		case PRE_TRAVEL:
			if (opt == 0) {
				pre_order_tree(root);
			} else {
				nore_pre_travel(root);
			}
			break;

		case MID_TRAVEL:
			if (opt == 0) {
				mid_order_tree(root);
			} else {
				nore_mid_travel(root);
			}
			break;

		case POST_TRAVEL:
			if (opt == 0) {
				post_order_tree(root);
			} else {
				nore_post_travel(root);
			}
			break;
	}

	while (queue_get_data(&data)) {
		printf("%d ", data);
	}
	printf("\n");
}


/*二叉树的层次遍历*/
int layer_order_tree(treenode_pt root, int *buff)
{
	treenode_pt node;
	int cnt = 0;

	if (root ==	NULL) {
		return 0;
	}


	clear_btqueue();
	en_btqueue(root);

	while (!btqueue_isempty()) {
		if (0 != out_btqueue(&node)) {    //按层次顺序提取节点数据
			buff[cnt] = node->tn_val;	
			cnt++;
		}

		if (node->tn_lchild != NULL) {
			en_btqueue(node->tn_lchild);
		}

		if (node->tn_rchild != NULL) {
			en_btqueue(node->tn_rchild);
		}
	}

	return cnt;
}


/*
 * @brief : 获取每一层节点个数
 * @param :
 *		root  :根节点
 *		buff  :接收每层节点个数缓冲区
 * @return: 返回层数(缓冲区有效长度)
 */
int layer_node_cnt(treenode_pt root, int *buff)
{
	treenode_pt node;
	int idx, n_cnt, next_cnt;
	int l_cnt;

	if (root ==	NULL) {
		return 0;
	}

	clear_btqueue();
	en_btqueue(root);
	n_cnt = 1;
	buff[0] = 1;

	l_cnt = get_tree_height(root);
	for (idx = 1; idx < l_cnt; idx++) {
		next_cnt = 0;
		while (n_cnt > 0) {
			if (0 != out_btqueue(&node)) { //出队当前层节点
				n_cnt--;
			}
			
			/*将下一层所有节点入队*/
			if (node->tn_lchild != NULL) {
				en_btqueue(node->tn_lchild);
				next_cnt++;
			}
			if (node->tn_rchild != NULL) {
				en_btqueue(node->tn_rchild);
				next_cnt++;
			}
		}
		n_cnt = next_cnt;
		buff[idx] = n_cnt;      //写入当前层节点数
	}

	return l_cnt;
}


/* Get the height of a binary tree. */
int get_tree_height(treenode_pt root)
{
	int left_h;
	int right_h;
	int height = 0;

	if (root != NULL) {
		left_h = get_tree_height(root->tn_lchild);
		right_h = get_tree_height(root->tn_rchild);

		height = (left_h > right_h) ? (left_h + 1) : (right_h + 1);
	}

	return height;
}

/* clear a binary tree. */
void clear_tree(treenode_pt root)
{
	if (root != NULL) {
		remove_tree(&root->tn_lchild);
		remove_tree(&root->tn_rchild);
		root->tn_lchild = NULL;
		root->tn_rchild = NULL;
	}

}

/* remove a binary tree. */
void remove_tree(treenode_pt *root)
{
	treenode_pt p_lchild, p_rchild;

	if (*root != NULL) {
		p_lchild = (*root)->tn_lchild;
		p_rchild = (*root)->tn_rchild;
		
		(*root)->tn_lchild = NULL;
		(*root)->tn_rchild = NULL;
		free((*root));

		remove_tree(&p_lchild);
		remove_tree(&p_rchild);
	}
}

/*
 * @brief : 根据值查找出现的节点
 * @param :
 *		root  :根节点
 *		p_node:接收查找的节点指针
 *		val   :查找的节点值
 * @return: Not.
 */
static void search_node(treenode_pt root, treenode_pt *p_node, int val)
{
	treenode_pt l_child;
	treenode_pt r_child;

	p_node = NULL;
	if (root == NULL) {	
		return;
	}

	if (root->tn_val == val) {
		*p_node = root;
		return;
	}

	l_child = root->tn_lchild;
	r_child = root->tn_rchild;
	if (l_child != NULL) {
		search_node(l_child, p_node, val);
	}
	if (r_child != NULL && p_node == NULL) {  //左子树没找到才查找右子树
		search_node(r_child, p_node, val);
	}
}


/*
 * @brief : 根据值查找出现的节点,的父节点
 * @param :
 *		root  :根节点
 *		p_node:接收查找的父节点指针
 *		val   :查找的节点值
 * @return: Not.
 */
void search_parent_node(treenode_pt root, treenode_pt *p_node, int val)
{
	treenode_pt l_child;
	treenode_pt r_child;

	p_node = NULL;
	if (root == NULL) {
		return;
	}

	l_child = root->tn_lchild;
	r_child = root->tn_rchild;
	if (l_child != NULL) {
		if (l_child->tn_val == val) {
			*p_node = root;
			return;
		}
		search_parent_node(l_child, p_node, val);
	}
	if (r_child != NULL && p_node == NULL) {
		if (r_child->tn_val == val) {
			*p_node = root;
			return;
		}
		search_parent_node(r_child, p_node, val);
	}
}

/*
 * @brief : 根据值查找出现的节点的兄弟节点
 * @param :
 *		root  :根节点
 *		val   :查找的节点值
 * @return:
 *		treenode_pt:返回查找节点的兄弟节点的根
 */
static treenode_pt search_brother_node(treenode_pt root, int val)
{
	treenode_pt p_node = NULL;
	treenode_pt l_child, r_child; 
	
	/* 获取节点的直接父节点*/
	search_parent_node(root, &p_node, val);

	if (p_node != NULL) {
		l_child = p_node->tn_lchild;
		r_child = p_node->tn_rchild;

		/* 当前节点非左即右*/
		if (l_child != NULL && val == l_child->tn_val) {
			if (r_child != NULL) {
				return r_child;
			}
		}
		if (r_child != NULL && val == r_child->tn_val) {
			if (l_child != NULL) {
				return l_child;
			}
		}
		
	}

	return NULL;
}


/*
 * @brief : 排序二叉树插入一个节点
 * @param :
 *		root :根节点指针
 *		n_val:插入节点的值
 * @return:
 *		1 :节点插入成功.
 *		0 :节点插入错误.
 */
int insert_node(treenode_pt *root, int in_val)
{
    if (*root == NULL) {
        *root = create_node(in_val);
        return 1;

    } else if ((*root)->tn_val > in_val) {
        return insert_node(&(*root)->tn_lchild, in_val);

    } else if ((*root)->tn_val < in_val) {
        return insert_node(&(*root)->tn_rchild, in_val);
    }

    return 0;
}


/*delete node from val*/
int delete_node(treenode_pt* p_root, int d_val)
{
    treenode_pt d_node = NULL;
    treenode_pt p_node = NULL;
	treenode_pt p_del = NULL;

	search_node(*p_root, &d_node, d_val);
    if (NULL == d_node){                       //节点不存在
        return 0;
    }
	search_parent_node(*p_root, &p_node, d_val);  //查找父节点

    if (d_node->tn_lchild == NULL) {           /* 情况1:要删除的节点没有左孩子，即只有右孩子或左右孩子都没有*/
        if (d_node == *p_root) {               //为根节点
            *p_root = d_node->tn_rchild;

		} else {
            if (d_node == p_node->tn_lchild) {  //不是根节点一定含有父节点,非左即右
                p_node->tn_lchild = d_node->tn_rchild;
            } else {
                p_node->tn_rchild = d_node->tn_rchild;
            }
        }

    }  else if (d_node->tn_rchild == NULL) {    /* 情况2:要删除的节点没有右孩子，即只有左孩子或左右孩子都没有*/
        if (d_node == *p_root) {
            *p_root = d_node->tn_lchild;

        } else {
            if (d_node == p_node->tn_lchild) {
                p_node->tn_lchild = d_node->tn_lchild;

            } else {
                p_node->tn_rchild = d_node->tn_lchild;
            }
        }

    }  else {                                 /* 情况3:删除的节点左右孩子都存在*/
        p_del = d_node->tn_rchild;
        p_node = d_node;
        while (p_del->tn_lchild) {          //在其右子树中找一个最小的节点，替代待删除节点
            p_node = p_del;
            p_del = p_del->tn_lchild;
        }
        d_node->tn_val = p_del->tn_val;

        if (p_del == p_node->tn_lchild) {
            p_node->tn_lchild = p_del->tn_rchild;
            
        }  else {    //右孩子不存在左节点
            p_node->tn_rchild = p_del->tn_rchild;
        }

        d_node = p_del;
        free(p_del);
    }

	return 1;
}


/*
 * @brief : 修改节点值
 * @param :
 *		root  :根节点
 *		o_val:被修改节点的值
 *		n_val:修改后的值
 * @return:
 *		1 :修改成功.
 *		0 :不存节点.
 */
int change_node_val(treenode_pt root, int o_val, int n_val)
{
	treenode_pt node = NULL;
	treenode_pt p_node = NULL;
    int change_pos = 0;

	if (NULL == root) {
		return 0;
	}
    search_node(root, &node, o_val);
    search_parent_node(root, &p_node, o_val);

    if (node == NULL) {        //节点不存在
        return 0;

    } else if (node->tn_lchild != NULL) {
        if (node->tn_lchild->tn_val > n_val) {
            change_pos = 1;    //表示该新的节点需要改变位置
        }

    } else if (node->tn_rchild != NULL) {
        if (node->tn_rchild->tn_val < n_val) {
            change_pos = 1;
        }
        
    } else if (p_node != NULL) {
        if (p_node->tn_lchild == node && p_node->tn_val < n_val) {
            change_pos = 1;

        } else if (p_node->tn_rchild == node && p_node->tn_val > n_val) {
            change_pos = 1;
        }
    }

    if (0 == change_pos) {
        node->tn_val = n_val;

    } else {
        if (0 == delete_node(&root, o_val)) {
            return 0;
        }
        if (0 == insert_node(&root, n_val)) {
            return 0;
        }
        
    }

	return 1;
}


/*
 * @brief : 根据类型(parent/brother)查找节点对应的值
 * @param :
 *		val :接收查找到的节点的值
 *		root:根节点
 *		find_val:当前的节点的值
 *		type:查找的类型(FIND_P/FIND_B)
 * @return:
 *		1 :存在节点.
 *		0 :不存节点.
 */
int find_node_val(int *val, treenode_pt root, int find_val, FIND_TYPE type)
{
	treenode_pt p_node = NULL;

	if (root ==	NULL) {
		return 0;
	}

	switch (type) {
		case FIND_P:
			search_parent_node(root, &p_node, find_val);
			if (NULL != p_node) {
				*val = p_node->tn_val;
				return 1;
			} 

			break;
		case FIND_B:
			p_node = search_brother_node(root, find_val);
			if (NULL != p_node) {
				*val = p_node->tn_val;
				return 1;
			} 
			break;
	}

	return 0;
}


