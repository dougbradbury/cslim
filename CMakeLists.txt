project(CSlim-project)
set(CSlim_version_major 0)
set(CSlim_version_minor 2)

cmake_minimum_required(VERSION 3.5)

if (EXISTS "${CMAKE_SOURCE_DIR}/cmake/Modules/")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
endif()

option(CSLIM_DISABLE_UNIT_TESTS  "Disable unit tests" OFF)
option(CSLIM_DISABLE_EXAMPLES  "Disable unit tests" OFF)
option(CSLIM_BUILD_AS_CPP  "Build CSlim as C++" OFF)

###############
# Conan support
###############

find_program (CONAN_BIN conan)
if(CONAN_BIN)
    message (STATUS "Found conan C++ package manager: ${CONAN_BIN}")
    find_file (CONANFILE NAMES "conanfile.txt" HINTS "${CMAKE_SOURCE_DIR}")
    if (CONANFILE)
        message (STATUS "Found ${CONANFILE}")
        if (NOT EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
            message(STATUS "Trying to execute 'conan install'")
            get_filename_component(CONANFILE_DIR ${CONANFILE} DIRECTORY)
            execute_process(COMMAND conan install ${CONANFILE_DIR} --build=missing)
        endif()
    endif()

    if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
        include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        message(STATUS "conan_basic_setup()")
        conan_basic_setup(TARGETS)
    else()
        message(WARNING "Please run 'conan install' if you plan to use conan")
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    #set(CMAKE_BUILD_TYPE Debug)
endif()

###############################
# Check compiler's capabilities
###############################

include (CheckCCompilerFlag)
if (CSLIM_BUILD_AS_CPP)
    include (CheckCXXCompilerFlag)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set (CMAKE_COMPILER_IS_CLANG true)
elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    set (CMAKE_COMPILER_IS_MSVC true)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
    find_program (LINKER_BIN gold)
    if(LINKER_BIN)
        set(LINKER_BIN "gold")
    else()
        find_program (LINKER_BIN ld.gold)
        if(LINKER_BIN)
            set(LINKER_BIN "ld.gold")
        else()
            set(LINKER_BIN "ld")
        endif()
    endif()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")

    CHECK_C_COMPILER_FLAG("-fuse-ld=${LINKER_BIN}" USE_LINKER_FLAG)
    if(USE_LINKER_FLAG)
        set (CMAKE_SHARED_LINKER_FLAGS "-fuse-ld=${LINKER_BIN} ${CMAKE_SHARED_LINKER_FLAGS}")
    endif()
elseif(CMAKE_COMPILER_IS_MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_SECURE_SCL=0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_DEPRECATE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_NONSTDC_NO_DEPRECATE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_SCL_SECURE_NO_DEPRECATE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Zi")
endif()

if (CSLIM_BUILD_AS_CPP)
    add_definitions(-DCPP_COMPILING)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")
endif()

message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
set (CMAKE_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_PREFIX}/include")
set (CMAKE_INSTALL_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib")
set (CMAKE_INSTALL_BINDIR "${CMAKE_INSTALL_PREFIX}/bin")
message(STATUS "CMAKE_INSTALL_INCLUDEDIR: ${CMAKE_INSTALL_INCLUDEDIR}")
message(STATUS "CMAKE_INSTALL_LIBDIR: ${CMAKE_INSTALL_LIBDIR}")
message(STATUS "CMAKE_INSTALL_BINDIR: ${CMAKE_INSTALL_BINDIR}")

add_subdirectory(src)

if (CSLIM_DISABLE_EXAMPLES)
    message(STATUS "Skipping examples")
else()
    add_subdirectory("fixtures")
endif()

if(CSLIM_DISABLE_UNIT_TESTS)
    message(STATUS "Skipping unit tests")
else()
    enable_testing()
    add_subdirectory(tests)
endif()
