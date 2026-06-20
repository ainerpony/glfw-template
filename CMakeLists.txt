cmake_minimum_required(VERSION 3.15)
project(OpenGL CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ── Setup default build type for Unix Makefiles ───────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

if(NOT EXEC_OUTPUT_DIR)
    set(EXEC_OUTPUT_DIR "${CMAKE_BINARY_DIR}" CACHE STRING "Choose the executable output dir" FORCE)
endif()

# ── Select the GLFW output subdirectory based on the compiler. ────────────────
if(MSVC)
    set(GLFW_LIB_SUBDIR "lib-msvc")
elseif(MINGW)
    set(GLFW_LIB_SUBDIR "lib-mingw-w64")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(GLFW_LIB_SUBDIR "lib-gnu")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(GLFW_LIB_SUBDIR "lib-clang")
else()
    set(GLFW_LIB_SUBDIR "lib-other")
    message(WARNING "The compiler is not listed; GLFW will be placed in build/glfw/lib-other")
endif()

set(GLFW_OUT "${CMAKE_BINARY_DIR}/glfw/${GLFW_LIB_SUBDIR}")
message(STATUS "GLFW output directory: ${GLFW_OUT}")

# ── GLFW build option ─────────────────────────────────────────────────────────
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# ── Add GLFW subdirectory ─────────────────────────────────────────────────────
add_subdirectory(thirdparty/glfw-3.4 "${GLFW_OUT}")

# ── Set GLFW target output directory to ${CMAKE_BINARY_DIR}/glfw/lib-xxx/ ─────
set_target_properties(glfw PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY                 "${GLFW_OUT}"
    LIBRARY_OUTPUT_DIRECTORY                 "${GLFW_OUT}"
    RUNTIME_OUTPUT_DIRECTORY                 "${GLFW_OUT}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG           "${GLFW_OUT}/Debug"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE         "${GLFW_OUT}/Release"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO  "${GLFW_OUT}/Release_Debug"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL      "${GLFW_OUT}/Release_Small"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG           "${GLFW_OUT}/Debug"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE         "${GLFW_OUT}/Release"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO  "${GLFW_OUT}/Release_Debug"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL      "${GLFW_OUT}/Release_Small"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG           "${GLFW_OUT}/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE         "${GLFW_OUT}/Release"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO  "${GLFW_OUT}/Release_Debug"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL      "${GLFW_OUT}/Release_Small"
)

# ── Main executable ───────────────────────────────────────────────────────────
add_executable(main
    src/main.cc
    thirdparty/glad/glad.c
)

# ── Add header path ───────────────────────────────────────────────────────────
target_include_directories(main PRIVATE
    thirdparty/glfw-3.4/include  # Provide <GLFW/glfw3.h>
    thirdparty                   # Provide "glad/glad.h"
)

if (WIN32)
    target_link_libraries(main PRIVATE
        glfw
        gdi32     # Windows GDI
    )
elseif (UNIX)
    target_link_libraries(main PRIVATE
        glfw
    )
endif()

# ── Set main executable output directory to `${CMAKE_BINARY_DIR}` ─────────────
if ("${EXEC_OUTPUT_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    set_target_properties(main PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY                "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_BINARY_DIR}/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_BINARY_DIR}/Release"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/Release_Debug"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_BINARY_DIR}/Release_Small"
    )

    # Copy shader files to executable output directory
    add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_CURRENT_SOURCE_DIR}/src/shader.vert"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/shader.frag"
                "$<TARGET_FILE_DIR:main>/" # Destination
        COMMAND ${CMAKE_COMMAND} -E echo "Copy shader files to: $<TARGET_FILE_DIR:main>/"
    )
else()
    set_target_properties(main PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY                "${EXEC_OUTPUT_DIR}"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${EXEC_OUTPUT_DIR}"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${EXEC_OUTPUT_DIR}"
            RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXEC_OUTPUT_DIR}"
            RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${EXEC_OUTPUT_DIR}"
        )
endif()