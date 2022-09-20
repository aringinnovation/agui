cmake_minimum_required(VERSION 3.23)
project(agui LANGUAGES CXX VERSION "1.0.0")

include(FetchContent)
add_definitions(-fPIC -Wno-pedantic -Wno-dangling-pointer)
find_package(OpenGL REQUIRED)

# GLAD
FetchContent_Declare(
        glad
        GIT_REPOSITORY "https://github.com/Dav1dde/glad"
        GIT_TAG "v0.1.36"
)
FetchContent_MakeAvailable(glad)

# STB
FetchContent_Declare(
        stb
        GIT_REPOSITORY "https://github.com/nothings/stb.git"
        GIT_TAG "8b5f1f37b5b75829fc72d38e7b5d4bcbf8a26d55"
)
FetchContent_Populate(stb)
set(STB_SOURCES ${stb_SOURCE_DIR})
add_library(stb SHARED ${stb_SOURCE_DIR}/stb_image.h
        ${stb_SOURCE_DIR}/stb_image_resize.h
        ${stb_SOURCE_DIR}/stb_image_write.h)
set_target_properties(stb PROPERTIES LINKER_LANGUAGE C)
target_compile_definitions(stb PRIVATE STB_IMAGE_IMPLEMENTATION)
set_target_properties(stb PROPERTIES VERSION 1
        PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}")

target_include_directories(stb PUBLIC "$<BUILD_INTERFACE:${stb_SOURCE_DIR}>"
        INTERFACE
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")


# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY "https://github.com/g-truc/glm"
        GIT_TAG "0.9.9.8"
)
FetchContent_MakeAvailable(glm)

## GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw"
        GIT_TAG "3.3.3"
)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
FetchContent_MakeAvailable(glfw)

## FreeType
FetchContent_Declare(
        freetype
        GIT_REPOSITORY "https://gitlab.freedesktop.org/freetype/freetype"
        GIT_TAG "VER-2-12-1"
)
FetchContent_MakeAvailable(freetype)

## IMGUI
FetchContent_Declare(
        imgui
        GIT_REPOSITORY "https://github.com/ocornut/imgui"
        GIT_TAG 3af9ac320d4afc708893a3d6ba5c94bf24bb0ab5 # Docking branch
)
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
    FetchContent_Populate(imgui)

    # Copy headers so we can use them in application
    file(GLOB imgui_src_headerfiles ${imgui_SOURCE_DIR}/*.h)
    file(GLOB imgui_src_backends ${imgui_SOURCE_DIR}/backends/*.h)
    # Freetype
    file(COPY ${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.h
            ${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp
            DESTINATION ${CMAKE_BINARY_DIR}/_imgui/imgui)
    add_compile_definitions(IMGUI_ENABLE_FREETYPE)
    # Common headers
    file(COPY ${imgui_src_headerfiles} DESTINATION ${CMAKE_BINARY_DIR}/_imgui/imgui)
    file(COPY ${imgui_SOURCE_DIR}/backends DESTINATION ${CMAKE_BINARY_DIR}/_imgui/imgui)
    include_directories(${CMAKE_BINARY_DIR}/_imgui)

    # Configure IMGui library
    add_library(imgui STATIC)
    target_sources(imgui PRIVATE ${imgui_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)
    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
    target_link_libraries(imgui PUBLIC
            glad
            glfw
            glm::glm
            ${OPENGL_LIBRARIES}
            freetype
            )
endif ()

add_library(${PROJECT_NAME} STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/agui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/agui/agui.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/helper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/agui/helper.hpp)

target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${stb_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION}
        PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}")

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>" "$<BUILD_INTERFACE:${imgui_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

add_subdirectory(example)