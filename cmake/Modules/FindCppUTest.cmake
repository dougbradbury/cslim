# - Try to find CppUTest
#
# The following variables are optionally searched for defaults:
#  CONAN_CPPUTEST_ROOT:    Base directory where all CppUTest components are found
#
# The following are set after configuration is done:
#  CppUTest_FOUND
#  CPPUTEST_INCLUDE_DIRS    --> CppUTest's include directories
#  CPPUTEST_LIBRARIES       --> CppUTest's static libraries

include(FindPackageHandleStandardArgs)

if (CONAN_CPPUTEST_ROOT)
    message(STATUS "CONAN_CPPUTEST_ROOT: ${CONAN_CPPUTEST_ROOT}")
    set(CONAN_CPPUTEST_ROOT "" CACHE PATH "Folder contains CppUTest")
endif()


find_path(CPPUTEST_INCLUDE_DIR "CppUTest/CommandLineTestRunner.h"
    PATHS ${CONAN_CPPUTEST_ROOT} "/usr/include")

find_library(CPPUTEST_LIBRARY "CppUTest"
    PATHS ${CONAN_CPPUTEST_ROOT} "/usr/lib"
    PATH_SUFFIXES "lib" "lib64")

find_library(CPPUTESTEXT_LIBRARY "CppUTestExt"
    PATHS ${CONAN_CPPUTEST_ROOT} "/usr/lib"
    PATH_SUFFIXES "lib" "lib64")

find_package_handle_standard_args(CppUTest DEFAULT_MSG
    CPPUTEST_INCLUDE_DIR
    CPPUTEST_LIBRARY
    CPPUTESTEXT_LIBRARY)

if (CppUTest_FOUND)
    set(CPPUTEST_INCLUDE_DIRS ${CPPUTEST_INCLUDE_DIR})
    set(CPPUTEST_LIBRARIES ${CPPUTEST_LIBRARY} ${CPPUTESTEXT_LIBRARY})
    if (WIN32)
        set(CPPUTEST_LIBRARIES ${CPPUTEST_LIBRARIES} "winmm.lib")
    endif()

    mark_as_advanced(CONAN_CPPUTEST_ROOT
        CPPUTEST_INCLUDE_DIR
        CPPUTEST_LIBRARY
        CPPUTESTEXT_LIBRARY)

    if(NOT TARGET CppUTest::CppUTest)
        add_library(CppUTest::CppUTest UNKNOWN IMPORTED)
        set_target_properties(CppUTest::CppUTest PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${CPPUTEST_INCLUDE_DIRS}")
        set_target_properties(CppUTest::CppUTest PROPERTIES
            IMPORTED_LOCATION ${CPPUTEST_LIBRARY})
        if (WIN32)
            set_target_properties(CppUTest::CppUTest PROPERTIES
                IMPORTED_LINK_INTERFACE_LIBRARIES "winmm.lib")
        endif()

        add_library(CppUTest::CppUTestExt UNKNOWN IMPORTED)
        set_target_properties(CppUTest::CppUTestExt PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${CPPUTEST_INCLUDE_DIRS}")
        set_target_properties(CppUTest::CppUTestExt PROPERTIES
            IMPORTED_LOCATION ${CPPUTESTEXT_LIBRARY})
    endif()
endif()
