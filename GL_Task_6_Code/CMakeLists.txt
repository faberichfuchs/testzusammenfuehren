cmake_minimum_required(VERSION 3.22)

if(APPLE)
    # Fix runtime path for installed executables
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
endif()

project(GCGProject_GL)
# Tell CMake to use the vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/fuchs/Documents/TU_Wien/SS2025/PTVC/packages/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

if(MSVC)
    # https://docs.microsoft.com/en-us/previous-versions/958x11bc(v=vs.140)
    add_compile_options("$<$<CONFIG:Debug>:/MTd>$<$<CONFIG:Release>:/MT>")
    add_compile_options("$<$<CONFIG:Debug>:/ZI>$<$<CONFIG:Release>:/Zi>")
    add_compile_options(/FS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /Qpar")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES "src/*.cpp" "src/*.h")
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
if(UNIX AND NOT APPLE)
    set(LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/linux" ".")
elseif(UNIX AND APPLE)
    set(LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/macos" ".")
elseif(WIN32)
    set(LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/windows" ".")
endif()

find_package(OpenGL REQUIRED)

# --- Assimp Setup START ---
# Prevent building Assimp tools and tests
#set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
#set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

# Add Assimp as subdirectory (expected in external/assimp)
#add_subdirectory(external/assimp)
# --- Assimp Setup END ---

find_package(assimp CONFIG REQUIRED)

set(LINK_LIBRARIES
        debug glfw3d optimized glfw3
        debug glew32sd optimized glew32s
        debug GCG_GL_Lib_Debug optimized GCG_GL_Lib_Release
        OpenGL::GL
        assimp::assimp  # <-- Added Assimp to link libraries
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBRARIES})

# IDE specific settings
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    set_property(DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
elseif(CMAKE_GENERATOR MATCHES "Xcode")
    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_GENERATE_SCHEME TRUE CMAKE_XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    set_property(DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY XCODE_STARTUP_PROJECT ${PROJECT_NAME})
endif()

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${X11_INCLUDE_DIR})
endif()

install(
        TARGETS ${PROJECT_NAME} CONFIGURATIONS Debug DESTINATION "debug"
)
install(
        TARGETS ${PROJECT_NAME} CONFIGURATIONS Release DESTINATION "release"
)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa -framework IOKit")
endif()
