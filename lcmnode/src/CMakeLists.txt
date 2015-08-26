#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
add_executable(lcmnode lcmnode.cpp)
target_link_libraries(lcmnode lcm lcmtypes_bot2-core tcpimage)
