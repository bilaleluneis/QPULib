project (QPULib)

cmake_minimum_required(VERSION 3.13)

# Enable Caching to speed up compilation
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)
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

set(QPU_INCLUDES

        # Commons
        Lib/Common/Heap.h
        Lib/Common/Seq.h
        Lib/Common/Stack.h

        # VideoCore
        Lib/VideoCore/Invoke.h
        Lib/VideoCore/Mailbox.h
        Lib/VideoCore/SharedArray.h
        Lib/VideoCore/VideoCore.h

        # Target
        Lib/Target/CFG.h
        Lib/Target/Emulator.h
        Lib/Target/Encode.h
        Lib/Target/Liveness.h
        Lib/Target/LiveRangeSplit.h
        Lib/Target/LoadStore.h
        Lib/Target/Pretty.h
        Lib/Target/ReachingDefs.h
        Lib/Target/RegAlloc.h
        Lib/Target/RemoveLabels.h
        Lib/Target/Satisfy.h
        Lib/Target/SmallLiteral.h
        Lib/Target/Subst.h
        Lib/Target/Syntax.h

        # Source
        Lib/Source/Cond.h
        Lib/Source/Float.h
        Lib/Source/Gen.h
        Lib/Source/Int.h
        Lib/Source/Interpreter.h
        Lib/Source/Pretty.h
        Lib/Source/Ptr.h
        Lib/Source/Stmt.h
        Lib/Source/StmtExtra.h
        Lib/Source/Syntax.h
        Lib/Source/Translate.h

        # Lib
        Lib/Kernel.h
        Lib/Params.h
        Lib/QPULib.h)

set(QPU_SOURCES

        # VideoCore
        Lib/VideoCore/Invoke.cpp
        Lib/VideoCore/Mailbox.cpp
        Lib/VideoCore/VideoCore.cpp

        # Target
        Lib/Target/CFG.cpp
        Lib/Target/Emulator.cpp
        Lib/Target/Encode.cpp
        Lib/Target/Liveness.cpp
        Lib/Target/LiveRangeSplit.cpp
        Lib/Target/LoadStore.cpp
        Lib/Target/Pretty.cpp
        Lib/Target/ReachingDefs.cpp
        Lib/Target/RegAlloc.cpp
        Lib/Target/RemoveLabels.cpp
        Lib/Target/Satisfy.cpp
        Lib/Target/SmallLiteral.cpp
        Lib/Target/Subst.cpp
        Lib/Target/Syntax.cpp

        # Source
        Lib/Source/Float.cpp
        Lib/Source/Gen.cpp
        Lib/Source/Int.cpp
        Lib/Source/Interpreter.cpp
        Lib/Source/Pretty.cpp
        Lib/Source/Stmt.cpp
        Lib/Source/Syntax.cpp
        Lib/Source/Translate.cpp

        # Lib
        Lib/Kernel.cpp)

set(QPU_TESTS

        Tests/VectorOperations.cpp
        Tests/Runner.cpp)

# Build QPU Shared Library Target
add_library(qpu SHARED ${QPU_SOURCES} ${QPU_INCLUDES})
target_include_directories(qpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Lib)

# Build Tests Target
add_executable(qpuTests ${QPU_TESTS} ${QPU_SOURCES} ${QPU_INCLUDES})
target_include_directories(qpuTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Lib)
target_link_libraries(qpuTests Boost::unit_test_framework)
target_compile_definitions(qpuTests PRIVATE BOOST_TEST_DYN_LINK)

