cmake_minimum_required(VERSION 3.22)

project(Lab1-myFTP-test)
enable_testing()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
add_executable(ftp_test test.cpp)
target_link_libraries(ftp_test GTest::gtest_main)
gtest_discover_tests(ftp_test)
