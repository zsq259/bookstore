cmake_minimum_required(VERSION 3.10.2)
project(bookstore)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -Ofast")

add_executable(code ${src_dir} src/main.cpp src/parser.cpp src/users.cpp src/books.cpp src/diary.cpp)
