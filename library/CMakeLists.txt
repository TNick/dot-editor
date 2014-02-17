# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "dot-editor" )
project( ${LOC_PRJ_NAME} )

add_definitions( -DDOTEDITOR_SHARED=1 )

# ============================================================================



# ----------------------------------------------------------------------------
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    ${LOC_PRJ_NAME}.cc
    usermsg.cc
    plginman.cc
    plugin.cc
    keywords.cc
    syntax.cc

    impl/implinterf.cc
    impl/implmanager.cc
    impl/gviz/gvzedge.cc
    impl/gviz/gvznode.cc
    impl/implementations/implgvizlib.cc
    impl/implementations/implprog.cc

    gui/aboutdlg.cc
    gui/codeeditor.cc
    gui/dotviewer.cc
    gui/guictrl.cc
    gui/mw.cc
    gui/settings.cc
    gui/usermsggui.cc
)
if (${DOTEDITOR_WIN32})
    set ( LIB_SOURCES ${LIB_SOURCES}
        winres.rc
    )
endif (${DOTEDITOR_WIN32})

set ( LIB_HEADERS
    ${LOC_PRJ_NAME}.h
    usermsg.h
    plginman.h
    plugin.h
    keywords.h
    syntax.h

    impl/implinterf.h
    impl/implmanager.h
    impl/gviz/gvzedge.h
    impl/gviz/gvznode.h
    impl/implementations/implgvizlib.h
    impl/implementations/implprog.h

    gui/aboutdlg.h
    gui/codeeditor.h
    gui/dotviewer.h
    gui/guictrl.h
    gui/mw.h
    gui/settings.h
    gui/usermsggui.h
)

set ( LIB_UIS
    gui/aboutdlg.ui
    gui/mw.ui
    gui/settings.ui
    gui/usermsggui.ui
)

set ( LIB_RES
    application.qrc
)

set (LIBS)



qt5_wrap_ui(LIB_UIS_SRC ${LIB_UIS})
qt5_add_resources(LIB_RES_SRC ${LIB_RES})

# qt5_wrap_cpp(outfiles inputfile ... OPTIONS ...)
# qt5_wrap_ui(outfiles inputfile ... OPTIONS ...)
# qt5_create_translation( qm_files directories ... sources ... ts_files ... OPTIONS ...)
# qt5_add_translation( qm_files ts_files ... )

add_library( ${LOC_PRJ_NAME} SHARED
    ${LIB_SOURCES}
    ${LIB_UIS_SRC}
    ${LIB_RES_SRC}
)

target_link_libraries( ${LOC_PRJ_NAME}
    ${LIBS}
)

qt5_use_modules(${LOC_PRJ_NAME} Core Widgets)


if (INCLUDE_OUTPUT_PATH)
    FOREACH (hfile ${LIB_HEADERS})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${hfile}" "${INCLUDE_OUTPUT_PATH}/${hfile}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    ENDFOREACH(hfile)
endif (INCLUDE_OUTPUT_PATH)
# ============================================================================

install(
    TARGETS ${LOC_PRJ_NAME}
    ARCHIVE
    DESTINATION lib
    COMPONENT applications)
install(
    TARGETS ${LOC_PRJ_NAME}
    RUNTIME
    DESTINATION bin
    COMPONENT libraries)
install(
    FILES ${LIB_HEADERS}
    DESTINATION include
    COMPONENT headers)
