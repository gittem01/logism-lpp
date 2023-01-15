cmake_minimum_required(VERSION 3.0)
project(LogiLogi VERSION 1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/GLFW)

include_directories(${CMAKE_SOURCE_DIR}/third_party/GLM/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/GLFW/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/stb_image)
include_directories(${CMAKE_SOURCE_DIR}/third_party/vulkan/include)

# put this before add_executable else get linker error
link_directories(${CMAKE_SOURCE_DIR}/third_party/vulkan/lib)

include_directories(include)

add_executable(LogiLogi main.cpp src/Shader.cpp include/Shader.h)

file(GLOB sourceFiles src/*.cpp src/ShaderSources/*.cpp)
file(GLOB headerFiles include/ShaderHeaders/*.h)
file(GLOB headerFiles include/*.h)

add_library(vkMines ${headerFiles} ${sourceFiles})

target_link_libraries(LogiLogi vkMines glfw)

if (APPLE)
	target_link_libraries(LogiLogi vulkan)
elseif(UNIX)
	target_link_libraries(LogiLogi vulkanUnx)
elseif(WIN32)
	target_link_libraries(LogiLogi vulkanWin)
endif()