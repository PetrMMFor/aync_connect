cmake_minimum_required(VERSION 3.10)
project(Finans)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
INCLUDE(conan)
conan_cmake_run(CONANFILE conanfile.txt
    BASIC_SETUP NO_OUTPUT_DIRS CMAKE_TARGETS
    BUILD missing)



set(CMAKE_CXX_STANDARD 11)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/rapidjson/include)
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")

file(GLOB FINANCE_FILES src/*.cpp)
add_library(finans_lib ${FINANCE_FILES})
target_link_libraries(finans_lib CONAN_PKG::boost pthread)

include_directories("headers/")
add_executable(Finans main.cpp ${FINANCE_FILES} headers/CSession.h src/CSession.cpp)
target_link_libraries(Finans finans_lib)

file(GLOB FINANCE_FILES_TEST tests/*.cpp)
add_executable(Finans_Tests ${FINANCE_FILES_TEST} tests/get_configTest.cpp)
target_link_libraries(Finans_Tests finans_lib CONAN_PKG::boost pthread CONAN_PKG::gtest)

