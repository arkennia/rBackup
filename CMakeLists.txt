cmake_minimum_required(VERSION 3.5)

project(rBackup LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS DBus REQUIRED)

add_executable(rBackup
  main.cpp
  mainwindow.cpp
  mainwindow.h
  backupjob.cpp
  backupjob.h
  utility.h
  mainwindow.ui
  manager.cpp
  manager.h
)

target_compile_options(rBackup PRIVATE -Wall -W -Wextra -pedantic -Wno-unused-parameter -Wno-old-style-cast -Wsign-compare  -O2 -pipe 
        -fno-plt -g -fwrapv -fomit-frame-pointer
)

target_link_libraries(rBackup PRIVATE Qt5::Widgets Qt5::DBus)
