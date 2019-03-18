include config.mk

FLAG = -W -lm

#目标文件声明
all:$(OBJS) $(TARGET)

#根据类型生成依赖进而编译生成.o文件，并且存放到OBJPATH目录下
$(OBJS):%.o:%.c
	@echo DIR $(DIR)
	@echo FILES $(FILES)
	$(CC) -c $(FLAG) -o $(OBJPATH)/$(notdir $@) $< 

#连接所有的二进制文件生成程序
$(TARGET):$(OBJPATH)
	$(CC) -o $@ $(OBJPATH)/*.o -lm

#如果存放二进制文件的目录不存在，则创建
$(OBJPATH):
	mkdir -p $(OBJPATH)
	
.PHONY:clean
clean:
	-rm $(OBJPATH)/*.o $(TARGET)

