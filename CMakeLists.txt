cmake_minimum_required(VERSION 3.0...3.29)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-DLOG_USE_COLOR)

add_compile_options(-Wall -Wno-unknown-warning-option -Wfatal-errors)

project(app LANGUAGES C)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/deps)

file(GLOB SOURCES1 "src/*.c")

file(GLOB LOG_LIB_SRC "deps/log/*.c")

add_library(log_lib STATIC ${LOG_LIB_SRC})

set(SOURCES ${SOURCES1})

include(FindPkgConfig)

find_package(SDL2 REQUIRED)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})

add_executable(app ${SOURCES})
target_link_libraries(app
  log_lib
  SDL2
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

message("STATUS" "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configuration types" FORCE)
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
