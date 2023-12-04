#set cmake minimum version
#建议使用大写 
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

# set the project name and version
PROJECT(Myiconv VERSION 1.0)

# 设置编译器gcc 和 c99标准; 
set(CMAKE_C_COMIPLER "gcc")
set(CMAKE_C_STANDARD 99)
# 如果是c++则可如下设置
# set(CMAKE_CXX_COMIPLER "g++")
# set(CMAKE_CXX_STANDARD 11)

#=============添加文件一般这里需要修改=============
# 头文件路径， Note: 新加文件夹，这里添加头文件路径
include_directories(
    "${PROJECT_SOURCE_DIR}/mysrc/include"
)

#添加add_dir子目录
ADD_SUBDIRECTORY("./mysrc")

# 应用逻辑源代码的路径 
aux_source_directory("./mysrc/src" DIR_MAIN)  # 这个只需指定到目录
# set(DIR_MAIN "./mysrc/src/main.c")  # 这个需要指定到.c

# 编译成可执行文件并放置在指定路径，如果不存在此目录，则会新建一个
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin) # 如果不指定，则在默认路径下
add_executable(myiconv ${DIR_MAIN}) # 生成可执行文件，在windows下就是myiconv.exe


# 这个需要放在add_executable()之后，因为只有生成了demo执行文件后，
# 才能将库连接信息添加进demo可执行文件中去
# 链接其他库文件  Note: 新加库的话，这里也需要做添加
link_directories(
	"${PROJECT_SOURCE_DIR}/build/lib"
)

#=============添加库一般这里需要修改=============
target_link_libraries(myiconv mysrc_lib)
#target_link_libraries(myiconv iconv)

# 添加链接库
IF (WIN32)
	MESSAGE(STATUS "Now is windows")
	find_library(ICONV_LIBRARY iconv)
	if (ICONV_LIBRARY)
    		target_link_libraries(myiconv ${ICONV_LIBRARY})
	else()
    		message(STATUS "No libiconv")    
	endif()
ELSEIF (UNIX)
	MESSAGE(STATUS "Now is UNIX-like OS's.")
ENDIF ()


