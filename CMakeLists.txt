cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(banking_app VERSION 1.0.0 DESCRIPTION "Banking Application")

set(CMAKE_MODULE_PATH 
    "${CMAKE_SOURCE_DIR}/third-party/CMake-codecov/cmake" 
    ${CMAKE_MODULE_PATH}
)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/banking)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/banking)


option(BUILD_TESTS "Build tests" ON)
option(BUILD_COVERAGE "Enable code coverage" OFF)

if(BUILD_TESTS)

    add_subdirectory(third-party/gtest)
    
    enable_testing()
    
    include_directories(
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        ${gmock_SOURCE_DIR}/include 
        ${gmock_SOURCE_DIR}
    )
    

    file(GLOB TEST_SOURCES tests/*.cpp)
    add_executable(banking_tests ${TEST_SOURCES})

    target_link_libraries(banking_tests
        account
        transaction
        gtest
        gmock
        gtest_main
    )
    
    # Регистрация тестов
    add_test(NAME banking_tests COMMAND banking_tests)

    if(BUILD_COVERAGE)
        set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build" FORCE)
        find_package(codecov REQUIRED)

        add_coverage(banking_tests)
        
        list(APPEND LCOV_REMOVE_PATTERNS 
            "'${PROJECT_SOURCE_DIR}/tests/*'"
            "'${PROJECT_SOURCE_DIR}/third-party/*'"
        )
        
        coverage_evaluate()
    endif()
endif()
