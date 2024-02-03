CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

OPTION(JSON_TEST "JSON_TEST" OFF)

project(json)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/json/json.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json.c

    ${CMAKE_CURRENT_SOURCE_DIR}/include/json/json_dump.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json_dump.c

    ${CMAKE_CURRENT_SOURCE_DIR}/include/json/json_binary.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json_binary.c
)

SOURCE_GROUP(json FILES ${SOURCES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/json)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

if(JSON_TEST)
    include(CTest)
    enable_testing()

    add_subdirectory(tests)
endif()