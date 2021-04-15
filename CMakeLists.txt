cmake_minimum_required(VERSION 3.20)
cmake_policy(VERSION 3.20)

project(static_dvigl)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MinSizeRel
        CACHE STRING "Choose the type of build (Debug or Release or MinSizeRel)"
        FORCE
    )
endif()

if(WIN32)
else()
    set(OpenGL_GL_PREFERENCE GLVND
        CACHE STRING
        "Linux-only: if GLVND, use the vendor-neutral GL libraries (default)."
    )
    set_property(CACHE OpenGL_GL_PREFERENCE PROPERTY STRINGS GLVND LEGACY)
endif()

# find_package(Threads REQUIRED)

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)


# find the required packages

# ------------------- PREFER STATIC LIBRARIES  ---------------------------
# cmake respects the order of extensions when looking for libraries
SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
# ------------------------------------------------------------------------


# ===================== ZLIB =============================================
find_package(ZLIB REQUIRED)
set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    ZLIB::ZLIB
)
# ===================== SDL2 =============================================
find_package(SDL2 CONFIG REQUIRED)
set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    SDL2::SDL2main
    SDL2::SDL2-static
)
# ===================== SDL2 net =========================================
find_package(sdl2-net CONFIG REQUIRED)
set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    SDL2::SDL2_net
)
# ===================== SDL2 image =======================================
find_package(sdl2-image CONFIG REQUIRED)
set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    SDL2::SDL2_image
)
# ===================== SDL2 ttf =========================================
find_package(sdl2-ttf CONFIG REQUIRED)
set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    SDL2::SDL2_ttf
)
# ===================== Vorbis ==============================================
find_package(Vorbis CONFIG REQUIRED)
set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    Vorbis::vorbis Vorbis::vorbisenc Vorbis::vorbisfile
)
# ===================== Ogg ==============================================
find_package(Ogg CONFIG REQUIRED)
set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    Ogg::ogg
)
# ===================== Opus ==============================================
find_package(Opus CONFIG REQUIRED)
set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    Opus::opus
)
# ===================== SDL2 mixer =======================================
find_package(sdl2-mixer CONFIG REQUIRED)
set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    SDL2::SDL2_mixer
)
# ===================== glm ==============================================
find_package(glm CONFIG REQUIRED)
set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    glm::glm
)
# ===================== png ==============================================
find_package(libpng CONFIG REQUIRED)
set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    png_static
)
# ===================== entt==============================================
find_package(EnTT CONFIG REQUIRED)
message(STATUS "EnTT version ${EnTT_VERSION}")
set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    EnTT::EnTT
)
# ===================== opengl ===========================================
find_package(OpenGL REQUIRED)
add_definitions(${OPENGL_DEFINITIONS})
# # ===================== glad ===========================================
# add_library(GLAD STATIC "src/glad.c")
# set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS}
#     GLAD
# )
# ===================== ozz ===========================================
# add_subdirectory("C:\\Users\\yoda\\proj\\ozz-animation" "ozz-animation/")
# set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
#     ozz_animation
#     ozz_geometry
# )
# ===================== BGFX ===========================================
# find_library()
find_library(BGFX_LIBRARY
    NAMES bgfxRelease
    HINTS "C:\\Users\\yoda\\proj\\bgfx\\.build\\win64_vs2019\\bin\\"
)
find_library(BX_LIBRARY
    NAMES bxRelease
    HINTS "C:\\Users\\yoda\\proj\\bgfx\\.build\\win64_vs2019\\bin\\"
)
find_library(BIMG_LIBRARY
    NAMES bimgRelease
    HINTS "C:\\Users\\yoda\\proj\\bgfx\\.build\\win64_vs2019\\bin\\"
)
find_library(BIMG_DECODE_LIBRARY
    NAMES bimg_decodeRelease
    HINTS "C:\\Users\\yoda\\proj\\bgfx\\.build\\win64_vs2019\\bin\\"
)

include_directories("C:\\Users\\yoda\\proj\\bgfx\\include\\" )
include_directories("C:\\Users\\yoda\\proj\\bx\\include\\" )
include_directories("C:\\Users\\yoda\\proj\\bx\\include\\compat\\msvc\\" )

set(DVIGL_RUNTIME_LIBS ${DVIGL_RUNTIME_LIBS} 
    ${BGFX_LIBRARY}
    ${BX_LIBRARY}
    ${BIMG_LIBRARY}
    ${BIMG_DECODE_LIBRARY}
)
# ========================================================================

set(NAME "dvigl")

file(GLOB SOURCE "src/${NAME}/*.h" "src/${NAME}/*.cpp")

add_library(${NAME} SHARED ${SOURCE})

if(WIN32)
    target_compile_options(${NAME} PRIVATE /MT)
endif()

target_link_libraries(${NAME} PRIVATE ${DVIGL_RUNTIME_LIBS} )

# # 3rd party libraries
# include(cmake/3rdparty.cmake)

if(WIN32)
    target_link_options(${NAME} PRIVATE /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
endif()

if(WIN32)
    set_target_properties(
        ${NAME}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    )
    set_target_properties(
        ${NAME}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}/bin"
    )
endif()

include_directories(${CMAKE_SOURCE_DIR}/includes )


SET(CMAKE_FIND_LIBRARY_SUFFIXES .dll .so ${CMAKE_FIND_LIBRARY_SUFFIXES})

set(NAME "game")
file(GLOB SOURCE "src/${NAME}/*.h" "src/${NAME}/*.cpp")
add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} PRIVATE dvigl)

if(WIN32)
    set_target_properties(
        ${NAME}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    )
    set_target_properties(
        ${NAME}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}/bin"
    )
endif()