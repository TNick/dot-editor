# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "dot-ed" )
project( ${LOC_PRJ_NAME} )

# ============================================================================



# ----------------------------------------------------------------------------
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    main.cc
)
set ( LIB_HEADERS
    
)
set (LIBS)

if (${DOTEDITOR_WIN32})
    set(WIN32_FLAG WIN32)
endif (${DOTEDITOR_WIN32})

add_executable(${LOC_PRJ_NAME} ${WIN32_FLAG}
    ${LIB_SOURCES}
)

target_link_libraries( ${LOC_PRJ_NAME}
    "dot-editor"
)

# Use the Widgets module from Qt 5.
qt5_use_modules(${LOC_PRJ_NAME} Core Widgets)

# ============================================================================

install(
    TARGETS ${LOC_PRJ_NAME}
    RUNTIME DESTINATION bin
    COMPONENT applications)
