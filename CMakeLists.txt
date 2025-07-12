cmake_minimum_required(VERSION 4.0.3)

# Project init
project(Ion VERSION 1.0 LANGUAGES C CXX)

# Set the compiler to clang++
set(CMAKE_CXX_COMPILER clang++)

# Use C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable warnings and debugging symbols
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

# Find ICU libraries (Unicode)
find_package(ICU REQUIRED COMPONENTS uc i18n)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

# Print LLVM config details
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Include LLVM headers and define LLVM-related macros
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# ICU include dirs
include_directories(${ICU_INCLUDE_DIRS})

# Source files
add_executable(Ion
  src/main.cpp

  # src/implementations/

  src/includes/ion.hh
  src/includes/constructs.hh
  src/includes/processes.hh
  src/includes/utils.hh
)

# Link against ICU libraries
target_link_libraries(Ion PRIVATE ICU::i18n ICU::uc)

# Link against LLVM libraries
llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(Ion PRIVATE ${llvm_libs})
