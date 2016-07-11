cmake_minimum_required(VERSION 3.5)
project(clang_test)

# Set the LLVM and Clang source code path
set(LLVM_PATH /home/uriel/CERN/llvm-3.8.0_clang-3.8.0/llvm-3.8.0.src)
set(CLANG_PATH ${LLVM_PATH}/tools/clang)

# Set the LLVM and Clang build dir path
set(LLVM_BUILD_PATH /home/uriel/CERN/llvm-3.8.0_clang-3.8.0/build)
set(CLANG_BUILD_PATH ${LLVM_BUILD_PATH}/tools/clang)

# Specify the paths in which the linker should search for libraries.
link_directories(${LLVM_BUILD_PATH}/lib ${CLANG_BUILD_PATH}/lib)

# Include path in which to search to resolve #include directive
include_directories(${LLVM_PATH}/include ${CLANG_PATH}/include)
include_directories(${CLANG_BUILD_PATH}/include ${LLVM_BUILD_PATH}/include)

# Set some flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti  -Wall -std=c++11")
set(CLANG_LIBRARIES clangFrontend
        clangSerialization
        clangDriver
        clangParse
        clangSema
        clangAnalysis
        clangAST
        clangBasic
        clangEdit
        clangLex
        clangTooling)
set(LLVM_LIBRARIES LLVMX86AsmParser
        LLVMX86Desc
        LLVMX86AsmPrinter
        LLVMX86Info
        LLVMX86Utils
        LLVMipo
        LLVMScalarOpts
        LLVMInstCombine
        LLVMTransformUtils
        LLVMAnalysis
        LLVMTarget
        LLVMOption
        LLVMMCParser
        LLVMMC
        LLVMObject
        LLVMBitReader
        LLVMCore
        LLVMSupport)
set(ALL_LIBRARIES ${CLANG_LIBRARIES} ${LLVM_LIBRARIES})

# Adds definitions to the compiler command line
add_definitions(
        -D__STDC_LIMIT_MACROS
        -D__STDC_CONSTANT_MACROS
)

# Set source files and set final target name.
add_executable(clang_test src/main.cpp)
add_executable(preprocessor_test src/preprocessorTest.cpp)

# Specify libraries or flags to use when linking a given target.
target_link_libraries(clang_test ${ALL_LIBRARIES})
target_link_libraries(preprocessor_test ${ALL_LIBRARIES})

include(FindCurses)
target_link_libraries(clang_test
        pthread
        #z
        dl
        ${CURSES_LIBRARIES}
        )