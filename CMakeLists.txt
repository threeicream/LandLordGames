cmake_minimum_required(VERSION 3.5)

set(EXE MainWindow) 
project(${EXE} LANGUAGES CXX)
 
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
 
find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)
 
#设置要包含的目录
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)
 
#由于采用set(CMAKE_AUTOUIC ON)的方式处理ui文件，编译时会报错找不到ui文件。
#需要将ui文件夹加入AUTOUIC的搜索目录，这样AUTOUIC处理时才能找到ui文件。
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)
 
file(GLOB PRO_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB PRO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB PRO_UI ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui)
file(GLOB PRO_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resource/*.qrc)
 
add_executable(${EXE}
${PRO_SRC}
${PRO_INCLUDE}
#${PRO_UI}
${PRO_RESOURCES}
 )
 
#制作静态库
#add_library(库名称 STATIC 源文件1 [源文件2] ...) 
#制作动态库
#add_library(库名称 SHARED 源文件1 [源文件2] ...) 
# 设置动态库/静态库生成路径
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#链接静态库
#link_libraries(<static lib> [<static lib>...])
#如果该静态库不是系统提供的，可以将静态库的路径也指定出来
#link_directories(<lib path>)
#链接动态库（在cmake中指定要链接的动态库的时候，应该将命令写到生成了可执行文件之后）（静态库也可以用这个，而且更推荐）
#target_link_libraries(
#    <target> 
#    <PRIVATE|PUBLIC|INTERFACE> <item>... 
#    [<PRIVATE|PUBLIC|INTERFACE> <item>...]...)
#如果该动态库不是系统提供的，可以将动态库的路径也指定出来
#link_directories(path)
 
target_link_libraries(${EXE} 
    PRIVATE 
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)