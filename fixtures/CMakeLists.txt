set(TARGET examples)
project(${TARGET})

set(fixtures_sources
    Main.c
    DecisionTableExample.c
    ExceptionsExample.c
    FixtureInCpp.cpp
    Fixtures.c
    QueryTableExample.c
    ScriptTableExample.c
    ScriptTableExampleEcho.c
    ScriptTableExampleEchoLoudly.c
    )
if (CSLIM_BUILD_AS_CPP)
    set_source_files_properties(${fixtures_sources} PROPERTIES LANGUAGE "CXX")
endif()

add_executable(${TARGET} ${fixtures_sources})

target_link_libraries(${TARGET} PUBLIC CSlim)
