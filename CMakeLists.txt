cmake_minimum_required(VERSION 3.26)

project(rt_terminal_color)

# Project template to make a cmake fetch-content-able library with CI/CD and testing triggers

option(RT_TERMINAL_COLOR_BUILD_TESTS "Build rt_terminal_color unit tests" OFF)

add_library(rt_terminal_color)

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
	add_subdirectory(dependencies)

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

# while this one is a header-only library, it's good to specify an install target
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