cmake_minimum_required(VERSION 3.12)

project(homework_6 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(ENABLE_WARNINGS "Enable compiler warnings" ON)
option(ENABLE_SANITIZERS "Enable sanitizers in Debug builds" ON)

add_executable(homework_6 main.cpp)

# Warnings
if(ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(homework_6 PRIVATE /W4)
    else()
        target_compile_options(homework_6 PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()

# Sanitizers (Debug only, non-MSVC)
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(NOT MSVC)
        target_compile_options(homework_6 PRIVATE -fsanitize=address,undefined)
        target_link_libraries(homework_6 PRIVATE -fsanitize=address,undefined)
    endif()
endif()


install(TARGETS homework_6 RUNTIME DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "homework_6")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Savchenko")  

include(CPack)




