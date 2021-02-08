cmake_minimum_required(VERSION 3.14)

project(PaciFontDesigner LANGUAGES CXX)

set(target_name "PaciFontDesigner")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core REQUIRED)

#list(APPEND QML_IMPORT_PATH ${PROJECT_SOURCE_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/pacifontdesigner BASE_SRCS)

add_library(${target_name} SHARED
  ${BASE_SRCS}
  paci_font_designer_resource.qrc
)

target_include_directories(${target_name}
    PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(${target_name}
  QoolUI
  Qt6::Core)

target_compile_definitions(${target_name} PRIVATE DLL_EXPORT)

if(WIN32)
  target_compile_definitions(${target_name} PRIVATE OS_WIN)
endif(WIN32)
