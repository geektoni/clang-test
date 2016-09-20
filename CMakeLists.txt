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
set(CMAKE_CXX_FLAGS "-fno-rtti -fPIC -fvisibility-inlines-hidden -Wall -W -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wno-missing-field-initializers -pedantic -Wno-long-long -Wno-maybe-uninitialized -Wno-comment -std=c++11 -ffunction-sections -fdata-sections -g")

# Set libraries required
set (CLANG_LIBRARIES
        clangTooling
        clangFrontendTool
        clangFrontend
        clangDriver
        clangSerialization
        clangCodeGen
        clangParse
        clangSema
        clangStaticAnalyzerFrontend
        clangStaticAnalyzerCheckers
        clangStaticAnalyzerCore
        clangAnalysis
        clangARCMigrate
        clangRewriteFrontend
        clangRewrite
        clangEdit
        clangAST
        clangLex
        clangBasic)

set(LLVM_LIBRARIES LLVMLTO
        LLVMObjCARCOpts
        LLVMLinker
        LLVMipo
        LLVMVectorize
        LLVMBitWriter
        LLVMIRReader
        LLVMAsmParser
        LLVMTableGen
        LLVMOption
        LLVMX86Disassembler
        LLVMX86AsmParser
        LLVMX86CodeGen
        LLVMSelectionDAG
        LLVMAsmPrinter
        LLVMX86Desc
        LLVMMCDisassembler
        LLVMX86Info
        LLVMX86AsmPrinter
        LLVMX86Utils
        LLVMMCJIT
        LLVMDebugInfoDWARF
        LLVMLineEditor
        LLVMInstrumentation
        LLVMInterpreter
        LLVMCodeGen
        LLVMTarget
        LLVMScalarOpts
        LLVMProfileData
        LLVMInstCombine
        LLVMOrcJIT
        LLVMTransformUtils
        #LLVMipa
        LLVMAnalysis
        LLVMRuntimeDyld
        LLVMExecutionEngine
        LLVMObject
        LLVMMCParser
        LLVMBitReader
        LLVMMC
        LLVMCore
        LLVMSupport
        )

include(FindCurses)
set(OTHER_LIBRARIES  pthread
        #z
        dl
        ${CURSES_LIBRARIES})

set(ALL_LIBRARIES ${CLANG_LIBRARIES} ${LLVM_LIBRARIES} ${OTHER_LIBRARIES})

# Adds definitions to the compiler command line
add_definitions(
        -D_GNU_SOURCE
        -D__STDC_CONSTANT_MACROS
        -D__STDC_FORMAT_MACROS
        -D__STDC_LIMIT_MACROS
)

# Set target install directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )

# Set source files and set final target name.
add_executable(ASTStatsPrinter src/ASTStatsPrinter.cpp)
add_executable(tokenPrinter src/tokenPrinter.cpp)
add_executable(preprocessor src/preprocessor.cpp lib/PrintPPOutputPPCallbacks.cpp)
add_executable(driver src/Driver.cpp)
add_executable(custolc src/CustomLanguage/custolc.cpp src/CustomLanguage/src/Lexer.cpp src/CustomLanguage/src/Parser.cpp
        src/CustomLanguage/src/AST.cpp)

# Specify libraries or flags to use when linking a given target.
target_link_libraries(ASTStatsPrinter ${ALL_LIBRARIES})
target_link_libraries(tokenPrinter ${ALL_LIBRARIES})
target_link_libraries(preprocessor ${ALL_LIBRARIES})
target_link_libraries(driver ${ALL_LIBRARIES})
target_link_libraries(custolc ${ALL_LIBRARIES})
