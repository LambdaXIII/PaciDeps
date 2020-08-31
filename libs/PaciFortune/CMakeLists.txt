cmake_minimum_required(VERSION 3.14)

project(PaciFortune LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(PaciFortune SHARED
  pacifortune/pacifortune_global.h
  pacifortune/pacifortune.cpp
  pacifortune/pacifortune.h
  fortune.qrc
)

target_link_libraries(PaciFortune Qt5::Core)
target_include_directories(PaciFortune INTERFACE ${PROJECT_SOURCE_DIR})

target_compile_definitions(PaciFortune PRIVATE DLL_EXPORT)
if(WIN32)
  target_compile_definitions(${target_name} PRIVATE OS_WIN)
endif(WIN32)
