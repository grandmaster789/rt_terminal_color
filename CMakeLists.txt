cmake_minimum_required(VERSION 3.26)

project(rt_terminal_color)

# Project template to make a cmake fetch-content-able library with CI/CD and testing triggers

set(CMAKE_CXX_STANDARD 23)

add_library(rt_terminal_color)
add_executable(rt_terminal_color_unittest)

add_subdirectory(dependencies)

file(GLOB_RECURSE rt_terminal_color_source CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
)

file(GLOB_RECURSE rt_terminal_color_unittest_source CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/test/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/test/*.inl"
)

target_sources            (rt_terminal_color PRIVATE ${rt_terminal_color_source})
target_include_directories(rt_terminal_color PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_sources            (rt_terminal_color_unittest PRIVATE ${rt_terminal_color_unittest_source})
target_include_directories(rt_terminal_color_unittest PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	"${CMAKE_CURRENT_SOURCE_DIR}/test"
)
target_link_libraries     (rt_terminal_color_unittest PRIVATE
	Catch2::Catch2WithMain
	rt_terminal_color
)

# should probably make testing optional
enable_testing()

# while this one is a header-only library, it's good to specify an install target
install(TARGETS rt_terminal_color LIBRARY DESTINATION lib)