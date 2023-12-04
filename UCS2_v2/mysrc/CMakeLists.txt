#set cmake minimum version
#建议使用大写 
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

# 指定头文件路径即mysrc下的include
include_directories("./include")

#添加add_dir子目录
#ADD_SUBDIRECTORY(lib)

## 如果bps/src中有10个.c文件，只需要添加5个到库中，则使用下面这种方法，挨个添加
# set(BSP_SRC "./src/main.c ./src/myiconv.c ./src/assert_coding.c")  # 这个需要指定到.c
## 如果bsp/src下所有的.c代码都要编译到库中，则推荐下面这个方式，只需要路径，不需要挨个添加
aux_source_directory("./src" ICONV_SRC)   # 这个只需指定到路径

# LIBRARY_OUTPUT_PATH是一个控制编译的变量，具体参考官方说明：
# https://cmake.org/cmake/help/v3.15/manual/cmake-variables.7.html
# 生成一个库并放置到指定目录下，如果此文件夹不存在则新建一个
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)	# 如果不指定，则默认路径下
add_library(mysrc_lib ${ICONV_SRC}) # 默认情况下是静态库 xxx.a

# 用到其他的代码文件时，即头文件引用了其他的头文件
# link_directories("${PROJECT_SOURCE_DIR}/lib")
# target_link_libraries(mysrc_lib iconv)


