set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
add_executable(lcmnode lcmnode.cpp TCPImageServer.cpp)
target_link_libraries(lcmnode lcm socket++ lcmtypes_bot2-core)