set(TARGET_CSlim CSlim)
project(${TARGET_CSlim})

if (UNIX)
    set(cslim_com_sources
        Com/SocketServer.c
        Com/TcpComLink.c
        )
else()
    set(cslim_com_sources
        ComWin32/SocketServer.c
        ComWin32/TcpComLink.c
        )
endif()

set(cslim_sources
    CSlim/ListExecutor.c
    CSlim/Slim.c
    CSlim/SlimConnectionHandler.c
    CSlim/SlimList.c
    CSlim/SlimListDeserializer.c
    CSlim/SlimListSerializer.c
    CSlim/SlimUtil.c
    ExecutorC/StatementExecutor.c
    ExecutorC/SymbolTable.c
    )

if (CSLIM_BUILD_AS_CPP)
    set_source_files_properties(${cslim_com_sources} PROPERTIES LANGUAGE "CXX")
    set_source_files_properties(${cslim_sources} PROPERTIES LANGUAGE "CXX")
endif()

add_library(${TARGET_CSlim} STATIC
    ${cslim_sources}
    ${cslim_com_sources}
    )

target_include_directories(${TARGET_CSlim} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/CSlim>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/ExecutorC>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/Com>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>  # <prefix>/include/CSlim
    )
if (WIN32)
    target_link_libraries(${TARGET_CSlim} PUBLIC WS2_32)
endif()

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/CSlim" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/ExecutorC" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/Com" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS "${TARGET_CSlim}"
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

set(TARGET_CSlim_main CSlim_main)
set(cslim_main_sources CSlim_main/Main.c)
if (CSLIM_BUILD_AS_CPP)
    set_source_files_properties(${cslim_main_sources} PROPERTIES LANGUAGE "CXX")
endif()
add_library(${TARGET_CSlim_main} STATIC ${cslim_main_sources})
target_link_libraries(${TARGET_CSlim_main} PUBLIC ${TARGET_CSlim})
install(TARGETS "${TARGET_CSlim_main}"
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(CMAKE_COMPILER_IS_MSVC)
    # TARGET_PDB_FILE is not supported with static libraries
    # https://gitlab.kitware.com/cmake/cmake/issues/16932
    #install(FILES $<TARGET_PDB_FILE:${TARGET_CSlim}> DESTINATION ${CMAKE_INSTALL_BINDIR} OPTIONAL)
    #install(FILES $<TARGET_PDB_FILE:${TARGET_CSlim_main}> DESTINATION ${CMAKE_INSTALL_BINDIR} OPTIONAL)

    # workaround
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_CSlim}.dir/$\{CMAKE_INSTALL_CONFIG_NAME\}/${TARGET_CSlim}.pdb"
        "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_CSlim_main}.dir/$\{CMAKE_INSTALL_CONFIG_NAME\}/${TARGET_CSlim_main}.pdb"
        DESTINATION ${CMAKE_INSTALL_LIBDIR} OPTIONAL)
endif()

