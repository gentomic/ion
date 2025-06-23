cmake_minimum_required(VERSION 3.16)
project(Ion LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

# ---- ICU ----
find_package(ICU REQUIRED COMPONENTS uc i18n)

# ---- LLVM ----
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# ---- Source files ----
add_executable(Ion 
  # Mains
  src/mains/aot.cpp

  # Headers
  src/ion.hh
  src/includes/constructs.hh
  src/includes/lexer.hh
  src/includes/processes.hh
  src/includes/utils.hh

  # Source Files
  src/implementations/utils.cpp
  src/implementations/lexer.cpp
)

# Link ICU libraries
find_package(ICU REQUIRED COMPONENTS uc i18n)

target_include_directories(Ion PRIVATE ${ICU_INCLUDE_DIRS})
target_link_libraries(Ion PRIVATE ${ICU_LIBRARIES})

# Link LLVM libraries (e.g., Support, Core, etc. as needed)
target_link_libraries(Ion PRIVATE LLVMCore LLVMSupport)
