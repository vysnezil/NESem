cmake_minimum_required(VERSION 3.10)

project(md5)

add_library(md5 src/md5.c include/md5.h)
target_include_directories(md5 PUBLIC include/)