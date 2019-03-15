/*****************************************************
 * Last modified : 2019-03-08 16:13
 * Filename      : bt_queue.c
 * Author        : Huruke
 * Email         : huruke@163.com
 * Description   : 提供模块之间数据信息的队列通信
 ****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "../inc/bt_queue.h" 

#define DATA_LEN      128L


static int data_queue[DATA_LEN] = {0};
static unsigned int write_idx = 0;
static unsigned int read_idx = 0;

static treenode_pt tree_queue[DATA_LEN];
static unsigned int in_idx = 0;
static unsigned int out_idx = 0;




void clear_queue(void)
{
	write_idx = read_idx;
}

/* @brief : data queue append data
 * @param :  
 * @return:
 *    1 : append success
 *    0 : append error
 */
int queue_append_data(int data)
{
	if ((write_idx+1) % DATA_LEN != read_idx) {
	
		data_queue[write_idx] = data;
		write_idx = (write_idx+1) % DATA_LEN;
		
		return 1;
	}

	return 0;
}

int queue_get_data(int *data)
{
	if (read_idx != write_idx) {
		*data = data_queue[read_idx];
		read_idx = (read_idx+1) % DATA_LEN;

		return 1;
	}

	return 0;
}

void queue_clear(void)
{
	read_idx = write_idx;
}

/*拷贝字符串到队列中*/
int queue_cpy_buff(int *buff, int len)
{
	int i = 0;

	for (i = 0; i < len; i++) {
		if (0 == queue_append_data(buff[i])) {
			return 0;    //@TODO:拷贝字符错误
		}
	}

	return 1;
}


void clear_btqueue(void)
{
	in_idx = out_idx;
}

int btqueue_isempty(void)
{
	if (in_idx == out_idx) {
		return 1;
	}

	return 0;
}

/*tree node enter the queue*/
void en_btqueue(treenode_pt node)
{
	if ((in_idx+1) % DATA_LEN != out_idx) {
	
		tree_queue[in_idx] = node;
		in_idx = (in_idx+1) % DATA_LEN;
	}
}



/*tree node output form queue*/
int out_btqueue(treenode_pt *node)
{
	if (out_idx != in_idx) {
		*node = tree_queue[out_idx];
		out_idx = (out_idx+1) % DATA_LEN;

		return 1;
	} else {
		*node = NULL;
	}
	return 0;
}


