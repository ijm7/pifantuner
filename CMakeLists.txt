cmake_minimum_required(VERSION 3.15)
project(pifantunerd CXX)




add_library(pifantunerd src/pifantunerd.cpp)
target_include_directories(pifantunerd PUBLIC include)



set_target_properties(pifantunerd PROPERTIES PUBLIC_HEADER "include/pifantunerd.h")
install(TARGETS pifantunerd)
