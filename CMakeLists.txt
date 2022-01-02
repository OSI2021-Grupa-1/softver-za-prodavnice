cmake_minimum_required(VERSION 3.19)

project (softver-za-prodavnice)



set(CMAKE_BUILD_TYPE Debug CACHE STRING "")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  message(STATUS "Exporting compile_commands.json")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# git
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  message(STATUS "Updating git submodules...")
  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    RESULT_VARIABLE UPDATE_SUBMODULES_RESULT
  )
  if(NOT UPDATE_SUBMODULES_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update failed!")
  endif()
endif()

#add library folder

set(FTXUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/ftxui)
add_subdirectory(${FTXUI_DIR})


# exe
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.?pp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${FTXUI_DIR}/include src include)

if(UNIX)
  target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

target_link_libraries(${PROJECT_NAME} 
                        PRIVATE ftxui::screen
                        PRIVATE ftxui::dom
  			                PRIVATE ftxui::component)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

enable_testing()
add_subdirectory(tests)
