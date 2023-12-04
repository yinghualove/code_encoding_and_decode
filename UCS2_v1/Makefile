#编译器
CC = gcc 
#CFLAGS = -std=gnu11  -c
#源代码c文件的路径
SRC_DIR = src
#头文件路径
INCLUDE_DIR = include
#编译生成的文件路径
BUILDDIR = build
#设置编译临时目录
OBJ_PATH:=$(BUILDDIR)/temp
#设置编译最终文件目录
BIN_PATH:=$(BUILDDIR)/bin
#库文件
LIBS += -liconv
#目标文件
TARGET = myiconv
#获取源文件目录下所有c文件列表
SRC:=$(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.c))
#去掉c文件目录
SRC_WITHOUT_DIR:=$(notdir $(SRC))
#生成.o文件列表
OBJ_WITHOUT_DIR:=$(patsubst %.c,%.o,$(SRC_WITHOUT_DIR))
#为.o文件列表加上编译目录
OBJ_WITH_BUILD_DIR:=$(addprefix $(OBJ_PATH)/,$(OBJ_WITHOUT_DIR))
$(info "OBJ_WITH_BUILD_DIR:$(OBJ_WITH_BUILD_DIR)")
#添加头文件目录
CFLAGS=$(addprefix -I,$(INCLUDE_DIR))
#为gcc添加源文件搜索目录
VPATH=$(SRC_DIR)
#编译目标
all:build_prepare $(TARGET)
#连接目标
$(TARGET):$(OBJ_WITH_BUILD_DIR) 
	cc -o $(BIN_PATH)/$@ $^ 
#编译生成.o文件
$(OBJ_PATH)/%.o:%.c
	cc -c $(CFLAGS) -o $@ $<
#创建编译目录
build_prepare:
	@if [ ! -d $(BUILDDIR) ]; then \
	mkdir -p $(BUILDDIR); \
	mkdir -p $(OBJ_PATH); \
	mkdir -p $(BIN_PATH); \
	fi


#OBJS = $(addprefix $(BUILDDIR)/, main.o myiconv.o assert_coding.o)

#$(BUILDDIR)/%.o: %.c
#	$(CC) $< $(CFLAGS) -o $@ 

#$(BUILDDIR)/$(TARGET):$(OBJS)
#	$(CC) $^ -lm -o $(BUILDDIR)/$(TARGET) $(LIBS)

.PHONY:clean	
clean:
#	rm -rf $(BUILDDIR)/*.o $(BUILDDIR)/$(TARGET)
		del /s /q $(BUILDDIR)\*.o
		del /s /q $(BUILDDIR)\$(TARGET).exe
		del /s /q $(BUILDDIR)
