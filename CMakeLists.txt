cmake_minimum_required (VERSION 3.14)

#SET(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Deployment target for OSX" FORCE)
project (PaciOneDeps)
add_definitions(-fPIC)

option(PREVIEW_LOCK "开启试用版时间锁机制")
string(TIMESTAMP TODAY "\"%Y-%m-%d\"" UTC)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/WarlockLib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/csv)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/uchardet-0.0.6)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/QoolUI/src)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/PaciBase)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/PaciCore)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/PaciFormat)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/PaciFortune)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/PaciFontDesigner)


#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/app/CaptionComposer EXCLUDE_FROM_ALL)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/app/AppIconStudio EXCLUDE_FROM_ALL)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/app/FFFever2 EXCLUDE_FROM_ALL)


#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/app/ColorAssistant EXCLUDE_FROM_ALL)


#list(APPEND QML_DIRS "${QML_IMPORT_PATH}")
list(APPEND QML_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/deps/QoolUI/src")
#list(APPEND QML_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/FFFever2")
list(APPEND QML_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/libs/PaciFontDesigner")
set(QML_IMPORT_PATH ${QML_DIRS} CACHE STRING "Creator qml path")
