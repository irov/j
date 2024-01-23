CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

project(json)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/json.h
    ${CMAKE_CURRENT_SOURCE_DIR}/json.c

    ${CMAKE_CURRENT_SOURCE_DIR}/json_dump.h
    ${CMAKE_CURRENT_SOURCE_DIR}/json_dump.c

    ${CMAKE_CURRENT_SOURCE_DIR}/json_binary.h
    ${CMAKE_CURRENT_SOURCE_DIR}/json_binary.c
)

SOURCE_GROUP(json FILES ${SOURCES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} STATIC ${SOURCES})