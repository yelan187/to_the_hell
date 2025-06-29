cmake_minimum_required(VERSION 3.28)
project(ToTheHell)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

# Check if local SFML source exists
if(EXISTS "${CMAKE_SOURCE_DIR}/xueba/SFML-2.6.2")
    FetchContent_Declare(SFML
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/xueba/SFML-2.6.2
        SYSTEM
    )
else()
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
    )
endif()

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

# 仅在 Windows 且使用 MinGW 时复制运行时 DLL
# cmake -G "MinGW Makefiles" ..
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    get_filename_component(MINGW_BIN "${CMAKE_CXX_COMPILER}" DIRECTORY)
    file(COPY
        "${MINGW_BIN}/libstdc++-6.dll"
        "${MINGW_BIN}/libgcc_s_seh-1.dll"
        "${MINGW_BIN}/libwinpthread-1.dll"
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endif()