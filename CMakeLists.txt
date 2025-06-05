cmake_minimum_required(VERSION 3.14...3.27)
project(solver VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CPACK_PACKAGE_NAME "solver")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION "Equation solver application")
set(CPACK_PACKAGE_CONTACT "walcheri <vagzvrf@mail.ru>")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "walcheri <vagzvrf@mail.ru>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.27)") 
set(CPACK_RPM_PACKAGE_RELEASE "1")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_REQUIRES "glibc >= 2.27")

include(CPack)

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

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "DEB;RPM;DragNDrop;WIX")
set(CPACK_PACKAGE_NAME "solver")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)
