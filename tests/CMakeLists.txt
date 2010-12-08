
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
add_definitions()
include_directories(
    ${CMAKE_SOURCE_DIR}/libs/hydrogen/include           # core headers
    ${CMAKE_BINARY_DIR}/libs/hydrogen/include           # generated config.h
    ${QT_INCLUDES}                                      # need this because of API exposures
)
link_directories()
add_executable(tests tests.cpp test_xml.cpp)
target_link_libraries(tests
    hydrogen-core
)
add_dependencies(tests hydrogen-core)

