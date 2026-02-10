cmake_minimum_required(VERSION 3.20)

project(app LANGUAGES CXX)

# ---------------- C++ ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- Sources ----------------
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
add_executable(app ${SRC_FILES})

# Include directories
target_include_directories(app PRIVATE headers)

# ---------------- SDL3 ----------------
# Prefer config mode (Windows, vcpkg, MSYS2)
find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(app PRIVATE SDL3::SDL3)

# ---------------- Vulkan ----------------
find_package(Vulkan REQUIRED)
target_include_directories(app PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(app PRIVATE ${Vulkan_LIBRARIES})

# ---------------- SDL3 GPU (optional) ----------------
# If you have SDL3 GPU installed separately
#find_package(SDL3_gpu CONFIG REQUIRED)
#target_link_libraries(app PRIVATE SDL3_gpu::SDL3_gpu)
#target_include_directories(app PRIVATE ${SDL3_gpu_INCLUDE_DIRS})

#----------------- OPENGL -----------------
find_package(OpenGL REQUIRED)
target_link_libraries(app PRIVATE OpenGL::GL dl)

# ---------------- Platform specifics ----------------
if (WIN32)
    # Windows OpenGL (if needed as fallback)
    target_link_libraries(app PRIVATE opengl32)
elseif (UNIX AND NOT APPLE)
    # Linux OpenGL fallback (optional)
    find_package(OpenGL REQUIRED)
    target_link_libraries(app PRIVATE OpenGL::GL dl)
endif()

# ---------------- Compiler warnings ----------------
if (MSVC)
    target_compile_options(app PRIVATE /W4)
else()
    target_compile_options(app PRIVATE -Wall -Wextra)
endif()
