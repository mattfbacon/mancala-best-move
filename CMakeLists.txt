cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME Mancala)
set(EXECUTABLE_NAME mancala)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configurations" FORCE)
set(CMAKE_POSITION_INDEPENDENT_CODE True)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
	-Wall -Wextra -pedantic \
	-Wno-enum-compare -Wno-gnu-zero-variadic-macro-arguments \
	-Wno-c++11-extensions -Wno-c++14-extensions \
	-Wno-c++17-extensions -Wno-c++20-extensions \
	-fuse-ld=lld \
	-fno-rtti -fno-exceptions \
	-Wno-unused-command-line-argument \
	-fopenmp \
	")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
	-fno-omit-frame-pointer -fstack-protector-strong \
	-Og -ggdb3 -Werror -Wfatal-errors \
	-D_GLIBCXX_DEBUG \
	")

option(USE_ASAN "Use Address Sanitizer (Debug Only)" ON)
if(${USE_ASAN})
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
		-fsanitize=address -fsanitize=undefined -fsanitize=leak \
		")
endif()

file(GLOB_RECURSE sources ${${PROJECT_NAME}_SOURCE_DIR}/src/**.cpp)

add_executable(${EXECUTABLE_NAME} ${sources})
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${${PROJECT_NAME}_SOURCE_DIR}/include)
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE ${${PROJECT_NAME}_SOURCE_DIR}/sysinc)
target_link_directories(${EXECUTABLE_NAME} PUBLIC ${${PROJECT_NAME}_SOURCE_DIR}/src)

set_property(TARGET ${EXECUTABLE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/dist")

# Extra Stuff Starts Here
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${${PROJECT_NAME}_SOURCE_DIR}/lib/magic_enum/include)
