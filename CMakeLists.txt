cmake_minimum_required(VERSION 3.20)
project(Multi3D)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++17 -Wall -O3")
set(BOOST_MIN_VERSION "1.70.0")

find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
  option(GIT_SUBMODULE "Check submodules during build" ON)
  if (GIT_SUBMODULE)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw
      RESULT_VARIABLE GIT_SUBMOD_RESULT
    )
    if (NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(FATAL_ERROR "git submodule update --init 
        --recursive failed with ${GIT_SUBMOD_RESULT},
        please checkout submodules")
    endif()
  endif()
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/extern")
add_subdirectory("${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE client_src "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE client_inc "${CMAKE_SOURCE_DIR}/include/")

add_executable(multi2D ${client_src})

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")

if (WIN32)

  set(CMAKE_EXE_LINKER_FLAGS "-static")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" CONFIG)
  set(BUILD_SHARED_LIBS OFF)

  message(STATUS "Windows Compilation Enabled")
  find_library(WSOCK32_LIBRARY wsock32 HINTS /usr/x86_64-w64-mingw32/lib/)
  find_library(WS2_32_LIBRARY ws2_32 HINTS /usr/x86_64-w64-mingw32/lib/)

  if (NOT WSOCK32_LIBRARY OR NOT WS2_32_LIBRARY)
    message(FATAL_ERROR "Could not find winsock libraries")
  else()
    message(STATUS "Windows libs found: ${WSOCK32_LIBRARY} ${WS2_32_LIBRARY}")
  endif()

  target_link_libraries(multi2D PUBLIC
                        ${WSOCK32_LIBRARY}
                        ${WS2_32_LIBRARY}
                        glad
                        glfw
                        glm::glm)
else()
  message(STATUS "Linux Compilation Enabled")
  target_link_libraries(multi2D PUBLIC
                        glad
                        glfw
                        glm::glm)
endif()

target_include_directories(multi2D PUBLIC
                           ${client_inc}
                           "${CMAKE_SOURCE_DIR}/include/"
                           "${CMAKE_SOURCE_DIR}/include/scenes/assets/"
                           "${CMAKE_SOURCE_DIR}/extern/glad/"
                           "${CMAKE_SOURCE_DIR}/extern/glm/glm/")