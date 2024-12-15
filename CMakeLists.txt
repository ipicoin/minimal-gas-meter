cmake_minimum_required(VERSION 3.20)
project(wasm_utils)

set(CMAKE_CXX_STANDARD 14)

find_package(WAVM REQUIRED CONFIG)

add_executable(wavm_gas_meter main.cpp simpleGas.c)
target_link_libraries(wavm_gas_meter WAVM::libWAVM)
