add_executable(example ${CMAKE_CURRENT_SOURCE_DIR}/example.cpp)
target_include_directories(example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(example agui)

add_custom_command(
        TARGET example PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/fonts
        ${CMAKE_CURRENT_BINARY_DIR}/fonts)

add_custom_command(
        TARGET example PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/img
        ${CMAKE_CURRENT_BINARY_DIR}/img)