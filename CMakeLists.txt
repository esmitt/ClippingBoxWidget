
cmake_minimum_required(VERSION 2.8)
 
PROJECT(BoxWidget2Clipping)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(BoxWidget2Clipping MACOSX_BUNDLE BoxWidget2Clipping.cpp )
 
target_link_libraries(BoxWidget2Clipping ${VTK_LIBRARIES})
