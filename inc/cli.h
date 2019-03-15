/*****************************************************
 * Last modified  : 2019-03-05 14:56
 * Filename       : cli.h
 * Author         : Huruke
 * Email          : huruke@163.com
 * Description    :
 ****************************************************/
#ifndef _CLI_H_
#define _CLI_H_

#include <stdio.h>
#include <stdlib.h>


#define CLI_FORMAT        ("BTREE>")        /* CLI interface prompt string */
#define CLI_INMAX         (1024L)

void cli_print(const char *str);
void cli_process(void);





#endif /* _CLI_H_ */
