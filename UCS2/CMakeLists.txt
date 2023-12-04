#set cmake minimum version
#建议使用大写 
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

# set the project name and version
PROJECT(Myiconv VERSION 1.0)

#添加add_dir子目录
#ADD_SUBDIRECTORY(lib)

#生成目标
SET(SRC_LIST main.c myiconv.c assert_coding.c)
ADD_EXECUTABLE(myiconv ${SRC_LIST})

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
	target_compile_options(myiconv PRIVATE -g)
ENDIF ()

