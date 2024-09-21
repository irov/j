CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

OPTION(JSON_TEST "JSON_TEST" OFF)
option(JSON_INSTALL "JSON_INSTALL" OFF)

project(json)

SET(JSON_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/json/json_config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/json/json.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/json/json_dump.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/json/json_binary.h
)

set(JSON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json_assert.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json_dump.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json_binary.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json_string.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json_string.c
)

SOURCE_GROUP(json FILES ${JSON_HEADERS} ${JSON_SOURCES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

if(JSON_TEST)
    include(CTest)
    enable_testing()

    add_subdirectory(tests)
endif()

if(JSON_INSTALL)
  install(TARGETS json
          EXPORT json_targets
          LIBRARY DESTINATION "lib"
          ARCHIVE DESTINATION "lib"
          RUNTIME DESTINATION "bin"
          INCLUDES DESTINATION "include")

  install(FILES ${JSON_HEADERS}
          DESTINATION "include")

  # Install exports for the install-tree.
  install(EXPORT json_targets
          NAMESPACE json::
          DESTINATION "${JSON_INSTALL_CMAKE_DIR}")
endif()

set(JSON_LIBRARIES json CACHE STRING "json libraries")