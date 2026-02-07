cmake_minimum_required(VERSION 3.14)
project(epson_projector_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

set(COMPONENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../components/epson_projector)

add_executable(epson_tests
    test_command.cpp
    test_response_parser.cpp
    test_command_queue.cpp
    ${COMPONENT_DIR}/command.cpp
    ${COMPONENT_DIR}/response_parser.cpp
    ${COMPONENT_DIR}/command_queue.cpp
)

target_include_directories(epson_tests PRIVATE
    ${COMPONENT_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
)

target_link_libraries(epson_tests GTest::gtest GTest::gtest_main)

gtest_discover_tests(epson_tests)
