cmake_minimum_required(VERSION 3.0.0)
project(myDocker VERSION 0.1.0)

include_directories("./CLI")
include_directories("./include")
aux_source_directory(. DIR_SRCS)

add_executable(myDocker ${DIR_SRCS})


