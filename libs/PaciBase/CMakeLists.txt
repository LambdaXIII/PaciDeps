cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
find_package(Qt5 COMPONENTS Core REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(PaciBase)

set(target_name "PaciBase")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pacibase/time_lock_defs.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/pacibase/time_lock_defs.h)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/pacibase BASE_SRCS)

add_library(${target_name} STATIC
  ${CMAKE_CURRENT_BINARY_DIR}/pacibase/time_lock_defs.h
  ${CMAKE_CURRENT_SOURCE_DIR}/pacibase/models/pa_filelistmodel.h
  ${BASE_SRCS}
)

target_include_directories(${target_name} PUBLIC ${PROJECT_SOURCE_DIR} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/pacibase)
#target_compile_definitions(${target_name} PRIVATE DLL_EXPORT)
target_link_libraries(${target_name} Qt5::Core libuchardet_static)

if(WIN32)
  target_compile_definitions(${target_name} PRIVATE OS_WIN)
endif(WIN32)

if(PREVIEW_LOCK)
  target_compile_definitions(${target_name} PRIVATE PREVIEW_LOCK)
endif(PREVIEW_LOCK)
