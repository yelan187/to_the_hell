cmake_minimum_required(VERSION 3.28)
project(ToTheHell)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(ToTheHell ${SOURCES} ${HEADERS})

target_compile_features(ToTheHell PRIVATE cxx_std_17)

target_include_directories(ToTheHell PRIVATE include)

target_link_libraries(ToTheHell PRIVATE 
    # SFML::Graphics
    # SFML::Window
    # SFML::System
    # SFML::Audio
    # SFML::Network 
    sfml-graphics
)