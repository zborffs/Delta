cmake_minimum_required(VERSION 3.13)
project(Delta)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Must include to avoid failing on Linux with std::thread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Import Packages
find_package(GTest REQUIRED) # Unit-Testing Library
# find_package(benchmark REQUIRED) # Benchmarking Library
find_package(spdlog REQUIRED) # Logging Library
find_package(yaml-cpp REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Matplot++ REQUIRED)

# Include Library Directories
include_directories(${GTEST_INCLUDE_DIR})
include_directories(${YAML_CPP_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
set(EIGEN_DIR /usr/local/include/eigen3)
include_directories(${EIGEN_DIR})
include_directories(include)
include_directories("../Cloak/include")

# Options
option(CODE_COVERAGE "Enable Code Coverage" OFF)
option(BUILD_TESTS "Build Tests" ON)
option(BENCH_TESTS "Build Benchmarks" OFF)
option(LOGGING "Enable Logging" ON) # Disable on Final Release

# If CODE_COVERAGE has been enabled, then update compilation flags
if(CODE_COVERAGE)
    message(STATUS "Enabling Code Coverage")
endif()

if (JETSON_BUILD)
    add_compile_definitions( JETSON_BUILD )
#    add_definitions( -DJETSON_BUILD )
#    get_directory_property( DirDefs DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS )
    message("-- Compiling for Jetson")
endif()

if (ASan)
    message("-- Compiling with Address Sanitizer")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
endif()

if (ThSan)
    message("-- Compiling with Thread Sanitizer")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=thread")
endif()

if (USan)
    message("-- Compiling with Undefined Sanitizer")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -pedantic") # -Werror
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g") # -O3
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O3")
endif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

# Put all source files excluding main.cpp into library source variable
set(DELTA_LIB_SRCS
        src/_app_state_init.cpp
        src/_app_state_eng.cpp
        src/_app_state_recognition.cpp
        src/_app_state_plan.cpp
        src/_app_state_wait.cpp
        src/_app_state_shutdown.cpp
        src/app.cpp
        src/controller.cpp
        src/engine.cpp
        src/hardware.cpp
        src/main.cpp
        src/model.cpp
        src/path_plan.cpp
        src/recognition.cpp
        src/state_estimator.cpp
)

# Create library, link external libraries to it
add_library(Delta-lib ${DELTA_LIB_SRCS})
set_target_properties(Delta-lib PROPERTIES PREFIX "")
link_directories(libs)
target_link_libraries(Delta-lib Matplot++::matplot Threads::Threads yaml-cpp spdlog::spdlog ${OpenCV_LIBS} libCloak-lib.a)
target_include_directories(Delta-lib PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/include ${SPDLOG_INCLUDE_DIR} ${YAML_CPP_INCLUDE_DIR})

# Create main executable
add_executable(Delta src/main.cpp)
target_link_libraries(Delta Delta-lib Threads::Threads yaml-cpp spdlog::spdlog ${OpenCV_LIBS} Matplot++::matplot)
install(TARGETS Delta DESTINATION bin)
install(TARGETS Delta-lib ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)

if (BUILD_TESTS)
    message("-- Building tests")
    add_subdirectory(test)
endif()