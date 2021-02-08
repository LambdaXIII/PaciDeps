cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Core Xml REQUIRED)
find_package(Qt6 COMPONENTS Core5Compat REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(PaciFormat CXX)

set(target_name "PaciFormat")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/paciformat CORE_SRCS)

add_library(${target_name} SHARED ${CORE_SRCS} format_resources.qrc)
target_link_libraries(${target_name}
  Qt6::Core Qt6::Xml
  Qt6::Core5Compat
  PaciCore PaciBase
#  warlock
  csv)

target_include_directories(${target_name}
  PUBLIC ${PROJECT_SOURCE_DIR})

target_compile_definitions(${target_name} PRIVATE DLL_EXPORT)

if(WIN32)
  target_compile_definitions(${target_name} PRIVATE OS_WIN)
endif(WIN32)
