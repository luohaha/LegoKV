# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# project
project (LegoKV)
if (${APPLE})
  set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -framework CoreFoundation")
else()
  set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -pthread -fPIC -no-pie -D_GLIBCXX_USE_CXX11_ABI=1")
endif()
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin) # Get full path to plugin
find_package(Protobuf REQUIRED)
function(PROTOBUF_GENERATE_GRPC_CPP SRCS HDRS)
  if(NOT ARGN)
    message(SEND_ERROR "Error: PROTOBUF_GENERATE_GRPC_CPP() called without any proto files")
    return()
  endif()

  if(PROTOBUF_GENERATE_CPP_APPEND_PATH) # This variable is common for all types of output.
    # Create an include path for each file specified
    foreach(FIL ${ARGN})
      get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
      get_filename_component(ABS_PATH ${ABS_FIL} PATH)
      list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
      if(${_contains_already} EQUAL -1)
          list(APPEND _protobuf_include_path -I ${ABS_PATH})
      endif()
    endforeach()
  else()
    set(_protobuf_include_path -I ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

  if(DEFINED PROTOBUF_IMPORT_DIRS)
    foreach(DIR ${Protobuf_IMPORT_DIRS})
      get_filename_component(ABS_PATH ${DIR} ABSOLUTE)
      list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
      if(${_contains_already} EQUAL -1)
          list(APPEND _protobuf_include_path -I ${ABS_PATH})
      endif()
    endforeach()
  endif()

  set(${SRCS})
  set(${HDRS})
  foreach(FIL ${ARGN})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
    get_filename_component(FIL_WE ${FIL} NAME_WE)

    list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.cc")
    list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.h")

    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.cc"
             "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.h"
      COMMAND  ${Protobuf_PROTOC_EXECUTABLE}
      ARGS --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
           --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
           ${_protobuf_include_path} ${ABS_FIL}
      DEPENDS ${ABS_FIL} ${Protobuf_PROTOC_EXECUTABLE}
      COMMENT "Running gRPC C++ protocol buffer compiler on ${FIL}"
      VERBATIM)
  endforeach()

  set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${SRCS} ${${SRCS}} PARENT_SCOPE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()

file(GLOB ProtoFiles "${PROJECT_SOURCE_DIR}/Proto/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})
PROTOBUF_GENERATE_GRPC_CPP(ProtoGRPCSources ProtoGRPCHeaders ${ProtoFiles})

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
Consensus/simple_paxos.cpp
Router/simple_dhash_impl.cpp
Proto/lkv_rpc.cpp
${ProtoSources}
${ProtoGRPCSources})
#client
add_executable(test_legokv_client 
Test/test_legokv_client.cpp
lkv.cpp
Proto/lkv_rpc.cpp
${ProtoSources}
${ProtoGRPCSources})
if (${APPLE})
  TARGET_LINK_LIBRARIES(test_legokv_client ${LibFiles})
  TARGET_LINK_LIBRARIES(test_legokv ${LibFiles})
else()
  TARGET_LINK_LIBRARIES(test_legokv_client ${LibFiles} ${LibFiles} libabsl_int128.a libabsl_raw_logging_internal.a)
  TARGET_LINK_LIBRARIES(test_legokv ${LibFiles} ${LibFiles} libabsl_int128.a libabsl_raw_logging_internal.a)
endif()
