cmake_minimum_required(VERSION 3.14)

project(static_todo VERSION "0.1.0" LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(IS_TOPLEVEL_PROJECT TRUE)
else()
    set(IS_TOPLEVEL_PROJECT FALSE)
endif()

option(STATIC_TODO_BUILD_EXAMPLES "Build static_todo examples" ${IS_TOPLEVEL_PROJECT})
option(STATIC_TODO_BUILD_TESTS "Build and perform static_todo tests" ${IS_TOPLEVEL_PROJECT})

if(STATIC_TODO_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(STATIC_TODO_BUILD_EXAMPLES)
    add_subdirectory(test)
endif()

option(STATIC_TODO_DISABLE "Explicitly disables static_todo functionality" OFF)
option(STATIC_TODO_ENABLE_DEADLINE_APPROACHING_WARNINGS "static_todo will issue \"deadline approaching warnings\" when a TODO_BEFORE will start to trigger errors in 10 days or fewer." ON)

if ("${STATIC_TODO_GIT_USERNAME}" STREQUAL "")
    execute_process(
        COMMAND git config --global user.name
        RESULT_VARIABLE
        SHORT_HASH_RESULT
        OUTPUT_VARIABLE STATIC_TODO_GIT_USERNAME
    )
    # Strip whitespace from GIT_USERNAME
    string(STRIP ${STATIC_TODO_GIT_USERNAME} STATIC_TODO_GIT_USERNAME)
endif ()
message(STATUS "static_todo using Git username: ${STATIC_TODO_GIT_USERNAME}")

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(${PROJECT_NAME} INTERFACE STATIC_TODO_GIT_USERNAME="${STATIC_TODO_GIT_USERNAME}")

if(STATIC_TODO_DISABLE)
    message(STATUS "Disabling static_todo")
    target_compile_definitions(${PROJECT_NAME} INTERFACE STATIC_TODO_DISABLE=1)
endif()

if(STATIC_TODO_ENABLE_DEADLINE_APPROACHING_WARNINGS)
    message(STATUS "Enabling static_todo \"deadline approaching\" warnings")
    target_compile_definitions(${PROJECT_NAME} INTERFACE STATIC_TODO_ENABLE_DEADLINE_APPROACHING_WARNINGS=1)
endif()
