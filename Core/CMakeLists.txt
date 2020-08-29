# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# project
project (LegoKV)
if (${APPLE})
  set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -framework CoreFoundation")
else()
  set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -pthread -fPIC -no-pie -D_GLIBCXX_USE_CXX11_ABI=0")
endif()
# include dir
include_directories(${PROJECT_SOURCE_DIR}/Base/ 
		    ${PROJECT_SOURCE_DIR} 
			${PROJECT_SOURCE_DIR}/Conf/
		    ${PROJECT_SOURCE_DIR}/Storage/
			${PROJECT_SOURCE_DIR}/Consensus/ 
			${PROJECT_SOURCE_DIR}/Router/
		    ${PROJECT_SOURCE_DIR}/LibsHeaders/
        ${PROJECT_SOURCE_DIR}/Proto/)
if (${APPLE})
  link_directories(${PROJECT_SOURCE_DIR}/../Libs)
  FILE(GLOB LibFiles ${PROJECT_SOURCE_DIR}/../Libs/*.a)
else()
  link_directories(${PROJECT_SOURCE_DIR}/../Libs-Linux)
  FILE(GLOB LibFiles ${PROJECT_SOURCE_DIR}/../Libs-Linux/*.a)
endif()
# target
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
if (${APPLE})
  TARGET_LINK_LIBRARIES(test_legokv_client ${LibFiles})
  TARGET_LINK_LIBRARIES(test_legokv ${LibFiles})
else()
  TARGET_LINK_LIBRARIES(test_legokv_client ${LibFiles} ${LibFiles} libabsl_int128.a libabsl_raw_logging_internal.a)
  TARGET_LINK_LIBRARIES(test_legokv ${LibFiles} ${LibFiles} libabsl_int128.a libabsl_raw_logging_internal.a)
endif()
