cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Core REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(PaciCore CXX)

set(target_name "PaciCore")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/pacicore CORE_SRCS)

add_library(${target_name} SHARED ${CORE_SRCS})
target_link_libraries(${target_name}
  Qt6::Core
  PaciBase
#  warlock
  )

target_include_directories(${target_name} PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_definitions(${target_name} PRIVATE DLL_EXPORT)

if(WIN32)
  target_compile_definitions(${target_name} PRIVATE OS_WIN)
endif(WIN32)
