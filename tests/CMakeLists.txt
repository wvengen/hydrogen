
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
add_definitions()
include_directories(
    ${CMAKE_SOURCE_DIR}/libs/hydrogen/include           # core headers
    ${CMAKE_BINARY_DIR}/libs/hydrogen/include           # generated config.h
)
link_directories()
add_executable(test_xml test_xml.cpp)
target_link_libraries(test_xml
    hydrogen-core
)
add_dependencies(test_xml hydrogen-core)

