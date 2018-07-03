include(CTest)

set(TARGET CSlim-tests)
project(${TARGET})

find_package(CppUTest REQUIRED)

if (CSLIM_BUILD_AS_CPP)
    set_source_files_properties(CSlim/TestSlim.c PROPERTIES LANGUAGE "CXX")
endif()

add_executable(${TARGET}
    AllTests.cpp
    CSlim/SlimListDeserializerTest.cpp
    CSlim/ListExecutorTest.cpp
    CSlim/SymbolTableTest.cpp
    CSlim/SlimConnectionHandlerTest.cpp
    CSlim/SlimListTest.cpp
    CSlim/SlimListSerializerTest.cpp
    CSlim/StatementExecutorTest.cpp
    CSlim/SlimUtilTest.cpp
    CSlim/TestSlim.c
    )
target_include_directories(${TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/CSlim")
target_link_libraries(${TARGET} PUBLIC CSlim CppUTest::CppUTest CppUTest::CppUTestExt)
#target_compile_options(${TARGET} PUBLIC
#    -Dmalloc=cpputest_malloc
#    -Dfree=cpputest_free
#    -Drealloc=cpputest_realloc
#    )
add_test(NAME ${TARGET} COMMAND ${TARGET} -v)
