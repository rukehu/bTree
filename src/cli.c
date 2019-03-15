/*****************************************************
 * Last modified : 2019-03-05 14:52
 * Filename      : cli.c
 * Author        : Huruke
 * Email         : huruke@163.com
 * Description   : 接收cli用户命令输入解析处理,实现cli命令行人机交互.
 ****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../inc/tree.h"
#include "../inc/bt_list.h"
#include "../inc/cli.h"
#include "../inc/bt_stack.h"


#define CLI_PARAM_CNT        (3L)   //CLI所支持的命令个数

static char cli_input[CLI_INMAX];   //接收cli字符输入缓冲区
static char data_buff[DATA_MAX];    //数据处理缓冲区

/* CLI command code. */
typedef enum cli_cmd_id {
	CMD_TREE_CREATE = 0,
	CMD_TREE_TRAVEL,
	CMD_TREE_REMOVE,
	CMD_TREE_CLEAR,
	CMD_TREE_HEIGHT,
	CMD_TREE_FIND,
	CMD_TREE_IN,
	CMD_TREE_SHOW,
	CMD_TREE_DNODE,
	CMD_TREE_CHANGE,
	CMD_TREE_LIST,
	CMD_CLI_HELP,
} CLI_CMD_ID;

/* CLI cmd string type struct.*/
typedef struct CMD_TABLE {
	char *cmd_str;
	CLI_CMD_ID id;
	char *cmd_help;
} CLI_CMD_TABLE_TYPE;

/* cli command mmaping table. */
static CLI_CMD_TABLE_TYPE cli_cmd_table[] = {
	{"create", CMD_TREE_CREATE, "<tree name> :create the tree."},
	{"rm",     CMD_TREE_REMOVE, "<tree name> :remove the tree."},
	{"clear",  CMD_TREE_CLEAR,  "<tree name> :clear the tree node val."},
	{"height", CMD_TREE_HEIGHT, "<tree name> :get the tree height."},
	{"travel", CMD_TREE_TRAVEL, "<tree name> <pre/mid/pos> [-n]:Traversal recursively by default,-n is Non-recursion."},
	{"find",   CMD_TREE_FIND,   "<tree name> <node val> <p/b>  :find node parent/brother for val."},
	{"del",    CMD_TREE_DNODE,  "<tree name> <node val> :delete node left/right child for val."},
	{"insert", CMD_TREE_IN,     "<tree name> <node val> :insert a value on the left/right of a node."},
	{"show",   CMD_TREE_SHOW,   "<tree name> display the tree graphics."},
	{"change", CMD_TREE_CHANGE, "<tree name> <node val> <new val > :change the tree node val."},
	{"ls",     CMD_TREE_LIST,   "print all tree list."},
	{"help",   CMD_CLI_HELP,    "[cmd] :print cmd list or how to use cmd."},
};


/* 删除字符串中存在的字符c */
static void strchar_d(char *str, int c)
{
	int i = 0;
	char *p = str;

	if (str == NULL) {
		return ;
	}

	while (*p != '\0') {
		if (*p != c) {
			str[i] = *p;
			i++;
		}	
		p++;
	}

	str[i] = '\0';
}


/*
 * @brief : 获取键盘输入一颗树的所有节点值.
 * @param :
 *		data : 数据接收缓冲区
 * @return: 接收到的数据长度
 */
static int get_clival_input(char *data)
{
	int in_len = 0;
	char *in_buff = cli_input;
	char val;
	int i;


	if (fgets(in_buff, DATA_MAX, stdin) != NULL) {
		/* 过滤换行符*/
		strchar_d(in_buff, '\r');
		strchar_d(in_buff, '\n');
		in_len = strlen(in_buff);

		for (i = 0; i < in_len; i++) {
			val = in_buff[i];
			/* 过滤其他输入字符*/
			if (('0' <= val && val <= '9') 
				|| val == '-' || val == ' ') {

				*data = val;
				data++;
			}
		}
		*data = '\0';
	}


	return in_len;
}


static void print_help(void)
{
	int i;
	CLI_CMD_TABLE_TYPE tab;

	printf("%-10s %-10s   %s    --try help param\n", "CMD", "param", "descreibe");

	for (i = 0; (unsigned)i < sizeof(cli_cmd_table)/sizeof(CLI_CMD_TABLE_TYPE); i++) {
		tab = cli_cmd_table[i];
		printf("%-10s %s\n", tab.cmd_str, tab.cmd_help);
	}
	printf("%s", CLI_FORMAT);
}


/* cli cmd format print. */
void cli_print(const char *str)
{
	if (str != NULL) {
		printf("%s\n\n", str);
		printf("%s", CLI_FORMAT);
	} else {
		printf("%s", CLI_FORMAT);
	}
}


/*指定一个长度输出该节点的字符格式*/
static void node_str_fomat(char *str, int node, int len)
{
	char node_s[12];
	int mid_str, mid_n;
	int node_len;
	int in_idx, i;

	if (str == NULL) {
		return;
	}
	memset(str, ' ', len);

	sprintf(node_s, "%d", node);
	node_len = strlen(node_s);

	if (node_len >= len) {
		strncpy(str, node_s, len);
		
	} else {
		/*获取整数写入位置*/
		mid_str = len/2;
		mid_n = node_len / 2;
		in_idx = mid_str - mid_n;

		for (i = 0; i < node_len; i++) {
			str[in_idx+i] = node_s[i];
		}
	}

	str[len] = '\0';
}


/*print the tree graphics.*/
void tree_print(treenode_pt root)
{
	int i, j, k;
	int tree_h;
	int ret;
	int node_cnt;
	int line_len, str_len;
	int in_idx;
	int space_len;
	int *cnt_buff = (int *)cli_input;

	char str[12];
	char *pos;
	int val;

	if (root == NULL) {
		return;
	}

	tree_h = layer_node_cnt(root, cnt_buff);

	/*层次遍历二叉树*/
	ret = layer_order_tree(root, node_buff);
	if (tree_h <= 0 || ret <= 0) {
		cli_print("not found the tree.");
		return;
	}

	/*获取节点的字符站位长度*/
	space_len = 1;
	for (i = 0; i < ret; i++) {
		sprintf(str, "%d", node_buff[i]);
		j = strlen(str);
		if (space_len < j) {    //节点输出站位宽度以最大值宽度为准
			space_len = j;
		}
	}

	/*先序遍历二叉树，并将二叉树值以最大占位格式转换存储*/
	mid_order_tree(root);
	in_idx = 0;
	while (queue_get_data(&val)) {
		node_str_fomat(str, val, space_len);
		for (i = 0; i < (int)strlen(str); i++) {
			data_buff[in_idx] = str[i];
			in_idx++;
		}
		data_buff[in_idx] = ' ';
		data_buff[++in_idx] = ',';
		in_idx++;
	}
	data_buff[in_idx - 1] = '\0';
	line_len = strlen(data_buff);

	in_idx = 0;
	for (i = 0; i < tree_h; i++) {
		node_cnt = cnt_buff[i];	        //获取当前层节点的个数
		memset(swp_buff, ' ', line_len);

		for (j = 0; j < node_cnt; j++) {
			node_str_fomat(str, node_buff[in_idx], space_len);
			in_idx++;
			pos = strstr(data_buff, str);
			k = 0;
			while (&data_buff[k] != pos) {         //获取节点具体位置
				k++;
			}

			str_len = strlen(str);
			while (str_len > 0) {
				swp_buff[k] = data_buff[k];
				k++;
				str_len--;
			}

			if (in_idx >= ret) {
				break;
			}
		}

		swp_buff[line_len] = '\0';
		printf("%s\n", swp_buff);
		printf("\n");
	}

}



/*
 * @brief : 用户界面命令输入处理函数.
 * @param :
 *		cmd      :命令字符串类型
 *		cmd_param:命令携带参数一
 *		cmd_rep  :命令参数二
 * @return: not
 */
static void cli_cmd_handle(const char *cmd, const char *cmd_param, const char *cmd_rep)
{
	int i;
	CLI_CMD_ID cmd_id;

	if (NULL == cmd) {
		return;
	}

	cmd_id = (CLI_CMD_ID)(-1);
	// lookup cmd into the cmd table
	for (i = 0; (unsigned)i < sizeof(cli_cmd_table)/sizeof(CLI_CMD_TABLE_TYPE); i++) {
		if (strcasecmp(cli_cmd_table[i].cmd_str, cmd) == 0) {
			cmd_id = cli_cmd_table[i].id;
			break;
		}
	}

	if (cmd_id == (CLI_CMD_ID)(-1)) {
		//@TODO: Maybe here notice user cmd not found.
		cli_print("cmd not found    --try help.");
		return;
	}

	switch (cmd_id) {
		case CMD_TREE_CREATE: {       //创建二叉树
			char in_data[32];
			int ret;
			int error_cnt = 0;
			btlist_pt l_node;

			if (cmd_param != NULL) {
				l_node = (btlist_pt)malloc(sizeof(btlist_t));
				strncpy(l_node->bt_name, cmd_param, NAME_MAX);
				l_node->bt_tree = (treenode_pt)malloc(sizeof(treenode_t)); 

				if (0 == append_btlist_node(l_node)) {
					free(l_node->bt_tree);
					free(l_node);
					cli_print("create tree error.");
					return;
				}

			} else {
				cli_print("param error please insert tree name.    --try help <param>.");
				return;
			}

			/* 一次性获取一颗树的所有节点*/
			cli_print("Please enter tree node valu split with Spac.");
			while (1) {
				ret = get_clival_input(in_data);
				if (ret > 0) {
					strncpy(swp_buff, in_data, ret);
					create_tree(&(l_node->bt_tree));
					break;
					
				} else if (error_cnt >= 2) {
					remove_btlist_btree(l_node->bt_name);
					cli_print("Eat more input error.");
					break;

				} else {
					printf("input tree node error.\n");
					printf("input example:-1 0 1 2 3 4...\n");
					cli_print("please enter agin...");
					error_cnt++;
				}
			}
			
			break;
		}
		case CMD_TREE_TRAVEL: {          //遍历二叉树
			treenode_pt bt_root;
			TRAVEL_TYPE type;
			char *p_save;
			char *str_tvl, *str_opt;
			int opt;


			if (cmd_param == NULL) {
				cli_print("travel param error   --try help <prama>");
				return;
			}

			bt_root = get_btlist_btree(cmd_param);
			if (bt_root == NULL) {
				cli_print("the tree is not found.");
				return;
			}

			strcpy(data_buff, cmd_rep);
			str_tvl = strtok_r(data_buff, " ", &p_save);
			str_opt = strtok_r(NULL, " ", &p_save);

			if (str_tvl == NULL) {
				cli_print("travel param error    --try help <travel>");
				return;
			} else if (strcasecmp(str_tvl, "pre") == 0) {
				type = PRE_TRAVEL;
			} else if (strcasecmp(str_tvl, "mid") == 0) {
				type = MID_TRAVEL;
			} else if (strcasecmp(str_tvl, "post") == 0) {
				type = POST_TRAVEL;
			} else {
				cli_print("travel param error    --try help <travel>");
				return;
			}

			if (str_opt == NULL) {
				opt = 0;         // 0默认使用递归方式,1选择非递归方式遍历
			} else if (strcasecmp(str_opt, "-n") == 0) {
					opt = 1;               
			} else {
				cli_print("travel param option err  --try help.");
			}

			travel_tree_node(bt_root, type, opt);
			break;
		}

		case CMD_TREE_REMOVE: {        //删除一颗二叉树
			if (cmd_param == NULL) {
				cli_print("remove param error   --try remove name");
				return;
			}

			if (0 != remove_btlist_btree(cmd_param)) {
				cli_print("remove tree ok.");
			} else {
				cli_print("remove tree error, No tree.");
			}

			break;
		}
		case CMD_TREE_CLEAR: {       //清空一颗二叉树
			if (cmd_param == NULL) {
				cli_print("remove param error   --try remove name");
				return;
			}

			if (0 != clear_list_btree(cmd_param)) {
				cli_print("clear tree ok.");
			} else {
				cli_print("clear tree error.");
			}

			break;
		}
		case CMD_TREE_HEIGHT: {    //获取二叉树高度
			int h;

			if (cmd_param == NULL) {
				cli_print("height param error   --try height name");
				return;
			}

			h = get_list_btheight(cmd_param);
			if (0 != h) {
				printf("the tree height = %d\n", h);
			} else {
				cli_print("No tree.");
			}

			break;
		 }
		case CMD_TREE_FIND: {     //查找二叉树的父/兄弟节点
			treenode_pt bt_root;
			FIND_TYPE type;
			char *p_save;
			char *str_val;
			char *str_type;
			int find_val, node_val;

			if (cmd_param == NULL || cmd_rep == NULL) {
				cli_print("find param error   --try help find");
				return;
			}
			//查找这颗树是否存在
			bt_root = get_btlist_btree(cmd_param);
			if (bt_root == NULL) {
				cli_print("the tree is not found.");
				return;
			}

			//获取查找节点值以及类型
			strcpy(data_buff, cmd_rep);
			str_val = strtok_r(data_buff, " ", &p_save);
			str_type = strtok_r(NULL, " ", &p_save);
			if (str_val == NULL || str_type == NULL) {
				cli_print("prama erro     --try <val> p/b.");
				return;
			}

			find_val = atoi(str_val);

			if (strcasecmp(str_type, "p") == 0) {
				type = FIND_P;
			} else if (strcasecmp(str_type, "b") == 0) {
				type = FIND_B;
			} else {
				cli_print("prama erro     --try <val> p/b.");
				return;
			}

			if (find_node_val(&node_val, bt_root, find_val, type) != 0) {
				if (type == FIND_P) {
					printf("parent node val = %d\n", node_val);
				} else {
					printf("brother node val = %d\n", node_val);
				}
			} else {
					printf("node not find.\n");
			
			}

			break;
		}
		case CMD_TREE_DNODE: {
			treenode_pt bt_root;
			int node_val;

			if (cmd_param == NULL || cmd_rep == NULL) {
				cli_print("delete param error   --try help.");
				return;
			}

			//查找树是否存在
			bt_root = get_btlist_btree(cmd_param);
			if (bt_root == NULL) {
				cli_print("the tree is not found.");
				return;
			}

			node_val = atoi(cmd_rep);    //节点值

			if (0 != delete_node(bt_root, node_val)) {
				cli_print("delete subtree ok.");
			} else {
				cli_print("the node is not found.");
			}

			break;
		}
		case CMD_TREE_IN: {        //插入节点
			treenode_pt bt_root;
			int in_val;

			if (cmd_param == NULL || cmd_rep == NULL) {
				cli_print("find param error   --try help find");
				return;
			}
			//查找这颗树是否存在
			bt_root = get_btlist_btree(cmd_param);
			if (bt_root == NULL) {
				cli_print("the tree is not found.");
				return;
			}

			//获取查找节点值以及类型
			in_val = atoi(cmd_rep);

			if (insert_node(bt_root, in_val) != 0) {
					printf("insert child node ok.\n");
			} else {
					printf("insert child node error.\n");
			}

			break;
	    }
		case CMD_TREE_SHOW: {
			treenode_pt bt_root;

			if (cmd_param == NULL) {
				cli_print("show tree param error   --try help find");
				return;
			}
			//查找这颗树是否存在
			bt_root = get_btlist_btree(cmd_param);
			if (bt_root == NULL) {
				cli_print("the tree is not found.");
				return;
			}

			tree_print(bt_root);

			break;
		}
		case CMD_TREE_CHANGE: {    //改变节点值
			treenode_pt bt_root;
			char *p_save;
			char *str_oval;
			char *str_nval;
			int o_val, n_val;

			bt_root = get_btlist_btree(cmd_param);
			if (bt_root == NULL) {
				cli_print("the tree is not found.");
				return;
			}

			strcpy(data_buff, cmd_rep);
			str_oval = strtok_r(data_buff, " ", &p_save);
			str_nval = strtok_r(NULL, " ", &p_save);
			if (str_oval == NULL || str_nval == NULL) {
				cli_print("prama erro     --try <node val> <new val>.");
				return;
			}
			o_val = atoi(str_oval);
			n_val = atoi(str_nval);

			if (change_node_val(bt_root, o_val, n_val) != 0) {
					printf("change node val ok.\n");
			} else {
					printf("change node val error.\n");
			}

			break;
	    }
		case CMD_TREE_LIST: {      //获取树列表
			char *name_buff[128];
			int tree_cnt = 0;
			int i;

			get_tree_list(name_buff, &tree_cnt);
			if (tree_cnt > 0) {
				for (i = 0; i < tree_cnt; i++) {
					printf("%s ", name_buff[i]);
				}
				printf("\n");
			} else {
				cli_print("no tree.");
			}

			break;
		}
		case CMD_CLI_HELP: {        //help命令帮助显示
			if (cmd_param == NULL) {
				print_help();
				return;
			}

			break;
		}
			
	}

}

/* cli用户命令输入字符串处理 */
static void cli_input_handle(void)
{
	int cnt = 0;
	char *enter_pos;
	char *p_save;
	char *p_ret;
	char *cmd = NULL;
	char *cmd_param = NULL;
	char *cmd_rep = NULL;

	/* @TODO: 在Linux下换行为\r,windwos下为\r\n,此处统一删除 */
	strchar_d(cli_input, '\r');
	strchar_d(cli_input, '\n');

	if (strlen(cli_input) > 0) {
		p_ret = strtok_r(cli_input, " ", &p_save);
		while (p_ret != NULL && (cnt < CLI_PARAM_CNT-1)) {
			if (cnt == 0) {             //提取cmd
				cmd = p_ret;

			} else if (cnt == 1) {      
				cmd_param = p_ret;      //提取第二个参数
				strcpy(swp_buff, p_save);
				cmd_rep = swp_buff;		//提取第三参数
			}

			p_ret = strtok_r(NULL, " ", &p_save);
			cnt++;
		}
		/* @TODO: 前两个参数不能含有空格 */
		strchar_d(cmd, ' ');
		strchar_d(cmd_param, ' ');

		cli_cmd_handle(cmd, cmd_param, cmd_rep);
	}
}


/* 命令行交互处理程序*/
void cli_process(void)
{
	btlist_init();      //初始化二叉树存储链表
	stack_init();       //初始化栈
	
	/* 任务处理主循环 */
  	while (1) {
		printf("%s", CLI_FORMAT);
		if (fgets(cli_input, CLI_INMAX, stdin) != NULL) {
			cli_input_handle();
		}
	}
}


