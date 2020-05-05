cmake_minimum_required(VERSION 3.5)

project(rBackup LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(rBackup
  main.cpp
  mainwindow.cpp
  mainwindow.h
  backupjob.cpp
  backupjob.h
  utility.h
  mainwindow.ui
)

target_compile_options(rBackup PRIVATE -Wall -W -Wextra -pedantic -Wno-unused-parameter -Wsign-compare  -march=native -O1 -pipe -fno-plt -g
    -fwrapv -fomit-frame-pointer
)

target_link_libraries(rBackup PRIVATE Qt5::Widgets)
