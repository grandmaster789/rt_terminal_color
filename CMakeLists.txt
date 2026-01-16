cmake_minimum_required(VERSION 3.20)

project(rt_terminal_color LANGUAGES CXX)

# Project template to make a cmake fetch-content-able library with CI/CD and testing triggers

option(RT_TERMINAL_COLOR_BUILD_TESTS "Build rt_terminal_color unit tests" OFF)

add_library(rt_terminal_color STATIC)
add_library(rt::terminal_color ALIAS rt_terminal_color)

file(GLOB_RECURSE rt_terminal_color_source CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
)

target_sources            (rt_terminal_color PRIVATE ${rt_terminal_color_source})
target_compile_features   (rt_terminal_color PUBLIC cxx_std_23)
target_include_directories(rt_terminal_color PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
	"$<INSTALL_INTERFACE:include>"
)

if (RT_TERMINAL_COLOR_BUILD_TESTS)
	enable_testing()

	# catch2 is only needed when tests are enabled
	add_subdirectory(dependencies EXCLUDE_FROM_ALL)

	add_executable(rt_terminal_color_unittest)

	file(GLOB_RECURSE rt_terminal_color_unittest_source CONFIGURE_DEPENDS
			"${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/test/*.h"
			"${CMAKE_CURRENT_SOURCE_DIR}/test/*.inl"
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
endif()

# Only set up installation rules when this is the top-level project
# (not when included via FetchContent or add_subdirectory)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	include(GNUInstallDirs)

	# install public headers
	# -- consider moving public headers to an "include/" directory
	install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
		FILES_MATCHING PATTERN "*.h" PATTERN "*.inl"
	)

	install(TARGETS rt_terminal_color
			EXPORT rt_terminal_colorTargets
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
	)

	install(EXPORT rt_terminal_colorTargets
			NAMESPACE rt::
			DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/rt_terminal_color"
	)
endif()