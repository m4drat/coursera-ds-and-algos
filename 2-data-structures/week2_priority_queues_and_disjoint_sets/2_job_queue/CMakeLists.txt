cmake_minimum_required(VERSION 3.18)
project(2_job_queue LANGUAGES CXX)

add_executable(${PROJECT_NAME} job_queue.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE "-stdlib=libc++")
target_link_options(${PROJECT_NAME} PRIVATE "-stdlib=libc++")