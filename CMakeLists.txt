cmake_minimum_required(VERSION 3.2)

project(HttpServer VERSION 1.0 LANGUAGES CXX)

# Configure output

#if(NOT DEFINED BUILD_MODE)
#	message(FATAL_ERROR "THe build mode wasn't specified.")
#endif()

set(BIN_OUTPUT_DIR "${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
set(LIB_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}")

# Configure flags

#if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
#	add_compile_options(-Og -Wall)
#
#elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
#	add_compile_options(-O3 -Wall)
#
#endif()

# Configure sublime text

if(DEFINED CONFIGURE_ST AND CONFIGURE_ST)
	message("Configuring sublime")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    add_compile_options(--verbose -target x86_64-pc-windows-gnu --sysroot=C:/Development/MinGW)
    #add_compile_options(--verbose)

endif()

# build
set(VCPKG_INSTALLED_DIR C:/Development/vcpkg/installed)
set(VCPKG_TRIPLET x64-windows)
set(BOOST_ROOT ${VCPKG_INSTALLED_DIR}/${VCPKG_TRIPLET})


find_package(Boost 1.82.0 REQUIRED)

add_executable(prog
			src/main.cpp
			src/HttpServer.cpp
			src/HttpConnection.cpp
			src/HttpRouter.cpp
			src/HttpController.cpp
			#src/example.cpp
			#src/router_test.cpp
			)

target_link_libraries(prog PUBLIC Boost::boost)

target_compile_features(prog PUBLIC cxx_std_17)
target_include_directories(prog PUBLIC "include")

set_target_properties( prog
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
	LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
	RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
)
