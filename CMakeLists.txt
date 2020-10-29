project (QPULib)

cmake_minimum_required(VERSION 3.13)

# Enable Caching to speed up compilation
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -fpermissive")

#add_compile_definitions(DEBUG) # ENABLE DEBUGGING LOGS
#add_compile_definitions(EMULATION_MODE) # USE EMULATION MODE
#add_compile_definitions(USE_SCALAR_VERSION) # THIS IS FOR main.cpp to use cpu version vs qpu version
add_compile_definitions(QPU_MODE) # USE ACTUAL HARDWARED QPU ON VPU OF THE GPU UP TO 12 CORES ARE AVAILABLE

find_package(Boost 1.67 REQUIRED COMPONENTS unit_test_framework)

enable_testing()

# QPU Library

set(QPU_INCLUDES)
set(QPU_SOURCES)
