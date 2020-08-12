# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (LuoKV)
set(CMAKE_CXX_FLAGS "-std=c++11 -O2")
# 添加include目录
include_directories(${PROJECT_SOURCE_DIR}/Base/ 
		    ${PROJECT_SOURCE_DIR} 
		    ${PROJECT_SOURCE_DIR}/Storage/ 
		    ${PROJECT_SOURCE_DIR}/LibsHeaders/)
link_directories(${PROJECT_SOURCE_DIR}/../Libs)
# 指定生成目标
add_executable(lkv lkv.cpp Base/base.cpp Storage/leveldb_impl.cpp)
TARGET_LINK_LIBRARIES(lkv libleveldb.a)
