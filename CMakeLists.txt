cmake_minimum_required(VERSION 3.10)
project(RaytracingGL)

add_subdirectory(libs/glad)
add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)

file(GLOB SOURCE_FILES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include/ libs/glfw/include/ libs/glm/)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad)

