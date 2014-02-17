# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "automata" )
project( ${LOC_PRJ_NAME} )

add_definitions( -DAUTOMATA_SHARED=1 )

set ( EXECUTABLE_OUTPUT_PATH  "${PROJECT_BINARY_DIR}/build/plugins" )
set ( LIBRARY_OUTPUT_PATH  "${PROJECT_BINARY_DIR}/build/plugins" )

# ============================================================================



# ----------------------------------------------------------------------------
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
	automatacomobj.cc
	cmnautomimpl.cc
	de-plgin-automata.cc
	dfautomata.cc
	enfautomata.cc
	nfautomata.cc
)

set ( LIB_HEADERS
	automatacomobj.h
	cmnautomimpl.h
	de-plgin-automata.h
	dfautomata.h
	enfautomata.h
	nfautomata.h
)

set (LIBS)

add_library( ${LOC_PRJ_NAME} SHARED
    ${LIB_SOURCES}
)

target_link_libraries( ${LOC_PRJ_NAME}
    ${LIBS}
	dot-editor
)

qt5_use_modules(${LOC_PRJ_NAME} Core)

if (INCLUDE_OUTPUT_PATH)
    FOREACH (hfile ${LIB_HEADERS})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${hfile}" "${INCLUDE_OUTPUT_PATH}/plugins/automata//${hfile}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    ENDFOREACH(hfile)
endif (INCLUDE_OUTPUT_PATH)
# ============================================================================

install(
    TARGETS ${LOC_PRJ_NAME}
    ARCHIVE
    DESTINATION lib
    COMPONENT plugins)
install(
    TARGETS ${LOC_PRJ_NAME}
    RUNTIME
    DESTINATION bin
    COMPONENT plugins)
install(
    FILES ${LIB_HEADERS}
    DESTINATION include/plugins/automata
    COMPONENT headers)
