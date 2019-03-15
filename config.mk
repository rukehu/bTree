#目标文件
TARGET=terminal

#所有的目录
DIR=./inc ./src .

#存放obj文件的目录
OBJPATH = ./OBJ

#获取目录下所有.c文件
FILES=$(foreach dir,$(DIR),$(wildcard $(dir)/*.c))

#将.c文件替换为.o文件
OBJS=$(patsubst %.c,%.o,$(FILES))
