cmake_minimum_required(VERSION 3.20)

project(TurretGame LANGUAGES C CXX)

add_compile_definitions(_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(SDL3_ROOT "C:/dev/SDL3-3.2.16" CACHE PATH "SDL3 install root")
set(GLM_DIR "C:/dev/glm" CACHE PATH "GLM root")
set(GLAD_DIR "C:/dev/glad" CACHE PATH "glad root (expects include/ and src/glad.c)")
set(STB_DIR "C:/dev/stb" CACHE PATH "stb root (header-only)")

list(APPEND CMAKE_PREFIX_PATH "${SDL3_ROOT}")

find_package(SDL3 CONFIG REQUIRED)

add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(glad SYSTEM PUBLIC ${GLAD_DIR}/include)

add_executable(TurretGame
    src/main.cpp
    src/Engine.h
    src/Engine.cpp
    src/Input.h
    src/Input.cpp
    src/Renderer.h
    src/Renderer.cpp
    src/World.h
    src/Entity.h
    src/Camera.h
    src/TextureManager.h
    src/TextureManager.cpp
    src/Texture.h
    src/Player.h
    src/Player.cpp
    src/Projectile.h
    src/Projectile.cpp
    src/World.cpp
    src/Random.h)

target_include_directories(TurretGame PRIVATE ${GLM_DIR} ${STB_DIR})

target_link_libraries(TurretGame PRIVATE
    SDL3::SDL3
    glad
)

if(MSVC)
    target_compile_options(TurretGame PRIVATE /W4 /permissive-)
    target_compile_definitions(TurretGame PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
else()
    target_compile_options(TurretGame PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
    add_custom_command(TARGET TurretGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3::SDL3>"
        $<TARGET_FILE_DIR:TurretGame>
    )

    add_custom_command(TARGET TurretGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        $<TARGET_FILE_DIR:TurretGame>/data
    )
endif()