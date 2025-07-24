cmake_minimum_required(VERSION 4.0)
project(renderer)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

find_package(glfw3 REQUIRED)
# find_package(OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendors/glad/include)
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/vendors/glad/src/gl.c src/*.cpp)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)


file(GLOB SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/*)
foreach(SHADER ${SHADERS})
    configure_file(${SHADER} res/shaders COPYONLY)
endforeach()
# add_custom_command(
#   TARGET ${PROJECT_NAME}
#   POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res"
#           "${CMAKE_CURRENT_BINARY_DIR}/res")
