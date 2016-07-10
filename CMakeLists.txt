cmake_minimum_required(VERSION 3.5)
project(clang_test)

set(LLVM_PATH /home/uriel/CERN/llvm-3.8.0_clang-3.8.0/build)
link_directories(${LLVM_PATH}/lib)
include_directories(${LLVM_PATH}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti  -Wall -std=c++11")

add_definitions(
        -D__STDC_LIMIT_MACROS
        -D__STDC_CONSTANT_MACROS
)

set(SOURCE_FILES src/main.cpp)
add_executable(clang_test ${SOURCE_FILES})

target_link_libraries(clang_test
        clangFrontend
        clangSerialization
        clangDriver
        clangParse
        clangSema
        clangAnalysis
        clangAST
        clangBasic
        clangEdit
        clangLex
        clangTooling
        )

target_link_libraries(clang_test
        LLVMX86AsmParser # MC, MCParser, Support, X86Desc, X86Info
        LLVMX86Desc # MC, Support, X86AsmPrinter, X86Info
        LLVMX86AsmPrinter # MC, Support, X86Utils
        LLVMX86Info # MC, Support, Target
        LLVMX86Utils # Core, Support
        LLVMipo
        LLVMScalarOpts
        LLVMInstCombine
        LLVMTransformUtils
        #LLVMipa
        LLVMAnalysis
        LLVMTarget
        LLVMOption # Support
        LLVMMCParser # MC, Support
        LLVMMC # Object, Support
        LLVMObject # BitReader, Core, Support
        LLVMBitReader # Core, Support
        LLVMCore # Support
        LLVMSupport
        )

include(FindCurses)
target_link_libraries(clang_test
        pthread
        #z
        dl
        ${CURSES_LIBRARIES}
        )

