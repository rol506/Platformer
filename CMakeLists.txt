cmake_minimum_required(VERSION 3.28.0 FATAL_ERROR)

set(PROJECT_NAME Platformer)
project(${PROJECT_NAME} VERSION 1.0)

add_executable(${PROJECT_NAME}
    src/main.cpp

    src/Renderer/ShaderProgram.cpp
    src/Renderer/ShaderProgram.h
    src/Renderer/Texture2D.cpp
    src/Renderer/Texture2D.h
    src/Renderer/Sprite2D.cpp
    src/Renderer/Sprite2D.h
    src/Renderer/VertexBuffer.cpp
    src/Renderer/VertexBuffer.h
    src/Renderer/IndexBuffer.cpp
    src/Renderer/IndexBuffer.h
    src/Renderer/VertexBufferLayout.cpp
    src/Renderer/VertexBufferLayout.h
    src/Renderer/VertexArray.cpp
    src/Renderer/VertexArray.h
    src/Renderer/Renderer.cpp
    src/Renderer/Renderer.h

    src/Resources/ResourceManager.cpp
    src/Resources/ResourceManager.h
    src/Resources/stb_image.h

    src/Physics/PhysicsEngine.cpp
    src/Physics/PhysicsEngine.h
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND}
    -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
