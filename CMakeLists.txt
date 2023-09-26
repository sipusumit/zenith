cmake_minimum_required(VERSION 3.8)

project(Zenith VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(LLVM_DIR "D:/ProgramFiles/LLVM/lib/cmake/llvm")

set(LLVM_ROOT_DIR "D:/ProgramFiles/LLVM")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINATIONS})

# Macros =============================================
add_compile_definitions(Z_VERSION="${CMAKE_PROJECT_VERSION}")
if(WIN32)
  add_compile_definitions(O_EXT="exe")
else()
  add_compile_definitions(O_EXT="out")
endif(WIN32)
# Main Application /===================================
add_executable(zenith src/main.cpp src/lexer.cpp src/driver.cpp src/parser.cpp src/utils.cpp src/AST.cpp)

target_include_directories(zenith PRIVATE include)

set(llvm_libs 
    LLVMX86TargetMCA
    LLVMMCA 
    LLVMX86Disassembler 
    LLVMX86AsmParser 
    LLVMX86CodeGen 
    LLVMX86Desc 
    LLVMX86Info 
    LLVMMCDisassembler 
    LLVMInstrumentation 
    LLVMGlobalISel 
    LLVMSelectionDAG 
    LLVMCFGuard 
    LLVMAsmPrinter 
    LLVMCodeGen 
    LLVMTarget 
    LLVMScalarOpts 
    LLVMInstCombine 
    LLVMAggressiveInstCombine 
    LLVMObjCARCOpts 
    LLVMTransformUtils 
    LLVMCodeGenTypes 
    LLVMBitWriter 
    LLVMAnalysis 
    LLVMProfileData 
    LLVMSymbolize 
    LLVMDebugInfoBTF 
    LLVMDebugInfoPDB 
    LLVMDebugInfoMSF 
    LLVMDebugInfoDWARF 
    LLVMObject 
    LLVMTextAPI 
    LLVMMCParser 
    LLVMMC 
    LLVMDebugInfoCodeView 
    LLVMIRReader 
    LLVMBitReader 
    LLVMAsmParser 
    LLVMCore 
    LLVMRemarks 
    LLVMBitstreamReader 
    LLVMBinaryFormat 
    LLVMTargetParser 
    LLVMSupport 
    LLVMDemangle
)

FUNCTION(PREPEND_LLVM var)
    set(listVar "")
    FOREACH(f ${ARGN})
        list(APPEND listVar "${LLVM_ROOT_DIR}/lib/lib${f}.a")
    ENDFOREACH(f)
    # message(STATUS "${files_to_install}")
    set(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND_LLVM)

PREPEND_LLVM(files_to_install ${llvm_libs})

target_link_libraries(zenith PRIVATE ${llvm_libs} )
if(WIN32)
  target_link_libraries(zenith PRIVATE pthread Shell32 ole32)
endif(WIN32)


# Install ======================================
install(TARGETS zenith 
        RUNTIME DESTINATION bin 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib
)

# only required if making a installer
# install(FILES ${files_to_install} DESTINATION bin)

# CPack ========================================

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)

# message(STATUS ${files_to_install})
