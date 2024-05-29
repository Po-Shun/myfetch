cmake_minimum_required(VERSION 3.12.0)

project(myfetch
        LANGUAGES C
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    message(STATUS "target system is LINUX")
    set(LINUX TRUE CACHE BOOL "..." FORCE)
endif()

include(CheckIncludeFile)

#############################
# Compile time dependencies #
#############################

set(THREADS_PREFER_PTHREAD_FLAG NOT WIN32)
find_package(Threads)

find_package(PkgConfig)
if(NOT PKG_CONFIG_FOUND)
    message(WARNING "pkg-config not found, library detection might be limited")
endif()

include(CheckIncludeFile)
#####################
# Configure options #
#####################

include(CMakeDependentOption)

# Compiler options

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

set(WARING_FLAGS "-Wall -Wextra -Wconversion -Werror=unintialized -Werror=return-type")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS} -Werror=incompatible-pointer-types -Werror=implicit-function-declaration -Werror=int-conversion")

# File system structre
if(NOT TARGET_DIR_ROOT)
    set(TARGET_DIR_ROOT "")
endif()

if(NOT TARGET_DIR_USR)
    set(TARGET_DIR_USR "${TARGET_DIR_ROOT}/usr")
endif()

if(NOT TARGET_DIR_HOME)
    set(TARGET_DIR_HOME "${TARGET_DIR_ROOT}/home")
endif()

if(NOT TARGET_DIR_ETC)
    set(TARGET_DIR_ETC "${TARGET_DIR_ROOT/etc}")
endif()

message(STATUS "Target dirs: ROOT=\"${TARGET_DIR_ROOT}\" USR=\"${TARGET_DIR_USR}\" HOME=\"${TARGET_DIR_HOME}\" ETC=\"${TARGET_DIR_ETC}\"")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${TARGET_DIR_USR}" CACHE PATH "..." FORCE)
endif()

if(NOT CMAKE_INSTALL_SYSCONFDIR)
    set(CMAKE_INSTALL_SYSCONFDIR "${TARGET_DIR_ETC}" CACHE PATH "..." FORCE)
endif()

# configure_file(src/myfetch_config.h.in myfetch_config.h @ONLY)

# Text data


function(myfetch_encode_c_string STR OUTVAR)
    string(REGEX REPLACE "\n$" "" TEMP "${STR}")  # Remove trailing newline
    string(REPLACE "\\" "\\\\" TEMP "${TEMP}")    # Escape backslashes
    string(REPLACE "\n" "\\n" TEMP "${TEMP}")     # Replace newlines with \n
    string(REPLACE "\"" "\\\"" TEMP "${TEMP}")    # Replace quotes with \"
    # PARENT_SCOPE set the variable in the scope of the calling function. That allows the variable could be accessed outside the func
    set(${OUTVAR} "\"${TEMP}\"" PARENT_SCOPE)
endfunction(myfetch_encode_c_string FILENAME OUTVAR)

function(myfetch_load_text FILENAME OUTVAR)
    file(READ "${FILENAME}" TEMP)
    myfetch_encode_c_string("$(TEMP)" TEMP)
    set(${OUTVAR} "${TEMP}" PARENT_SCOPE)
endfunction(myfetch_load_text)


# Ascii image data
file(GLOB LOGO_FILES "src/logo/ascii/*.txt")
set(LOGO_BUILTIN_H "#pragma once\n#pragma GCC diagnostic ignored \"-Wtrigraphs\"\n\n")
foreach(file ${LOGO_FILES})
    myfetch_load_text("${file}" content)
    get_filename_component(file "${file}" NAME_WE)
    string(TOUPPER "${file}" file)
    string(REGEX REPLACE "\\$\\{c([0-9]+)\\}" "$\\1" content "${content}")
    set(LOGO_BUILTIN_H "${LOGO_BUILTIN_H}#define FASTFETCH_DATATEXT_LOGO_${file} ${content}\n")
endforeach()
file(GENERATE OUTPUT logo_builtin.h CONTENT "${LOGO_BUILTIN_H}")

set(LIBMYFETCH_SRC)

if(LINUX)
    list(APPEND LIBMYFETCH_SRC)
endif()

# uncomment below once adding libraries

# add_library(libfastfetch OBJECT
# ${LIBFASTFETCH_SRC}
# )

# if(LINUX)
#     target_link_libraries(libfastfetch
#         PRIVATE "m"
#     )
# endif()

# target_include_directories(libfastfetch
#     PUBLIC ${PROJECT_BINARY_DIR}
#     PUBLIC ${PROJECT_SOURCE_DIR}/src
# )

# target_link_libraries(libfastfetch
#     PRIVATE ${CMAKE_DL_LIBS}
# )

add_executable(myfetch
    src/myfetch.c
)

# install target
install(
    TARGETS myfetch
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
)