cmake_minimum_required(VERSION 3.26)

project(main)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PRIVATE ../include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_definitions(${PROJECT_NAME} PRIVATE IMGDIR="../img/")

# For macOS M2 systems with SDL3 installed via Homebrew
list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake/SDL3" "/opt/homebrew/lib/cmake/SDL3")
list(APPEND CMAKE_PREFIX_PATH "/usr/local" "/opt/homebrew")

find_package(SDL3 QUIET)

if (SDL3_FOUND)
    message("Using SDL3 from find_package")
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL3_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_LIBRARIES})
else()
    # Fall back on pkg-config
    include(FindPkgConfig)
    pkg_search_module(SDL3 REQUIRED sdl3)
    message("Using SDL3 from pkg-config")
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL3_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_LIBRARIES})
endif()
