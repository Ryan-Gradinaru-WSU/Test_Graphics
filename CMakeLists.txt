cmake_minimum_required(VERSION 3.20)

project(app LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
     src/*.cpp)

# Create executable
add_executable(app ${SRC_FILES})

# Include directories
target_include_directories(app PRIVATE
    headers
)

# ---------------- SDL3 ----------------

# Prefer config mode (Windows, vcpkg, MSYS2)
find_package(SDL3 CONFIG REQUIRED)

target_link_libraries(app PRIVATE SDL3::SDL3)

# ---------------- Platform specifics ----------------

if (WIN32)
    # OpenGL on Windows
    target_link_libraries(app PRIVATE opengl32)
elseif (UNIX AND NOT APPLE)
    # Linux OpenGL
    find_package(OpenGL REQUIRED)
    target_link_libraries(app PRIVATE OpenGL::GL dl)
endif()

# Warnings (compiler-aware)
if (MSVC)
    target_compile_options(app PRIVATE /W4)
else()
    target_compile_options(app PRIVATE -Wall -Wextra)
endif()
