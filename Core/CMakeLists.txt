# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (LegoKV)
set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -framework CoreFoundation")
# 添加include目录
include_directories(${PROJECT_SOURCE_DIR}/Base/ 
		    ${PROJECT_SOURCE_DIR} 
			${PROJECT_SOURCE_DIR}/Conf/
		    ${PROJECT_SOURCE_DIR}/Storage/
			${PROJECT_SOURCE_DIR}/Consensus/ 
			${PROJECT_SOURCE_DIR}/Router/
		    ${PROJECT_SOURCE_DIR}/LibsHeaders/
                    ${PROJECT_SOURCE_DIR}/Proto/)
link_directories(${PROJECT_SOURCE_DIR}/../Libs)
FILE(GLOB LibFiles ${PROJECT_SOURCE_DIR}/../Libs/*.a)
# 指定生成目标
add_executable(test_legokv 
Test/test_legokv.cpp 	
lkv.cpp 
Base/base.cpp 
Conf/json_conf_impl.cpp
Storage/leveldb_impl.cpp
Consensus/simple_consensus_impl.cpp
Router/simple_dhash_impl.cpp
Proto/lkv.pb.cc
Proto/lkv.grpc.pb.cc
Proto/lkv_rpc.cpp)
#client
add_executable(test_legokv_client 
Test/test_legokv_client.cpp
lkv.cpp
Proto/lkv_rpc.cpp
Proto/lkv.pb.cc
Proto/lkv.grpc.pb.cc)
TARGET_LINK_LIBRARIES(test_legokv_client ${LibFiles})
TARGET_LINK_LIBRARIES(test_legokv ${LibFiles})

