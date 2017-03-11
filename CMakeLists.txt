# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# ============================================================================


# ----------------------------------------------------------------------------
# customisation

set ( PROJECT_NAME dot-editor )

# Automatically link Qt executables to qtmain target on Windows
cmake_policy(SET CMP0020 OLD)
# ============================================================================


# ----------------------------------------------------------------------------
# configure the project

project("dot-editor-package")

string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_U)
string(TIMESTAMP DOTEDITOR_BUILD_TIME UTC)

# may be either Release or Debug
if ( CMAKE_BUILD_TYPE STREQUAL "" )
    set ( CMAKE_BUILD_TYPE "Release" )
endif (CMAKE_BUILD_TYPE STREQUAL "")

# the version as known to CMake
set ( DOTEDITOR_MAJOR_VERSION 1)
set ( DOTEDITOR_MINOR_VERSION 1)
set ( DOTEDITOR_PATCH_VERSION 3)
set ( DOTEDITOR_VERSION
  "${DOTEDITOR_MAJOR_VERSION}.${DOTEDITOR_MINOR_VERSION}.${DOTEDITOR_PATCH_VERSION}")

# set some paths
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/build/bin" )
set ( EXECUTABLE_OUTPUT_PATH  "${PROJECT_BINARY_DIR}/build/bin" )
set ( LIBRARY_OUTPUT_PATH  "${PROJECT_BINARY_DIR}/build/lib" )
set ( INCLUDE_OUTPUT_PATH  "${PROJECT_BINARY_DIR}/build/include/dot-editor" )

include_directories(
  "${PROJECT_SOURCE_DIR}"
  "${PROJECT_BINARY_DIR}/build/include"
)

if (WIN32)
  # set stuff for windows
  set( DOTEDITOR_WIN32 ON )
else (WIN32)
  # set stuff for other systems

endif (WIN32)

# configure compiler
if ( CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)" )
    #add_definitions(/W4)
else ( CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)" )
    if (WIN32)
    else (WIN32)
        add_definitions(-Wall)
    endif (WIN32)
endif(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")

if   (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DQT_DEBUG=1 -D_DEBUG=1 )
    set ( DOTEDITOR_DEBUG ON )
    set ( DOTEDITOR_NDEBUG OFF )
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DQT_NO_DEBUG=1 -D_NDEBUG=1 )
    set ( DOTEDITOR_DEBUG OFF )
    set ( DOTEDITOR_NDEBUG ON )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
set ( CMAKE_DEBUG_POSTFIX  "_debug")

# Find includes in corresponding build directories
set ( CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set ( CMAKE_AUTOMOC ON)

# accumulate testing source files
set (modules_testing_src)


# ============================================================================


# ----------------------------------------------------------------------------
# external libraries

# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Widgets)


if (Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# ============================================================================

# ----------------------------------------------------------------------------
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/build/include/dot-editor/config.h"
)
# ============================================================================

# ----------------------------------------------------------------------------
add_subdirectory( "library" )
add_subdirectory( "editor" )
add_subdirectory( "plugins" )
# ============================================================================

# ----------------------------------------------------------------------------
# internal tests

# ============================================================================

# ----------------------------------------------------------------------------
# install

INCLUDE(InstallRequiredSystemLibraries)


IF(WIN32 AND NOT UNIX)

    file(WRITE "${PROJECT_BINARY_DIR}/qt.conf"
        "[Paths]\nPlugins=./plugins\n")

    set(CPACK_GENERATOR NSIS)
    SET(CPACK_NSIS_MODIFY_PATH OFF) # add to PATH variable

    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    SET(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/library/resources\\\\appico.ico")
    SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\dot-ed.exe")
    #SET(CPACK_NSIS_DISPLAY_NAME "Dot Editor")
    SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\sites.google.com/site/doteditorhome")
    SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\sites.google.com/site/doteditorhome")
    SET(CPACK_NSIS_CONTACT "nicu.tofan@gmail.com")

    STRING(REGEX REPLACE "moc.exe$" "" QT_BIN_PATH ${QT_MOC_EXECUTABLE})
    install(
        FILES
             ${QT_BIN_PATH}/icudt51.dll
             ${QT_BIN_PATH}/icuin51.dll
             ${QT_BIN_PATH}/icuuc51.dll
             ${QT_BIN_PATH}/libgcc_s_dw2-1.dll
             ${QT_BIN_PATH}/libstdc++-6.dll
             ${QT_BIN_PATH}/libwinpthread-1.dll
             ${QT_BIN_PATH}/Qt5Core.dll
             ${QT_BIN_PATH}/Qt5Gui.dll
             ${QT_BIN_PATH}/Qt5Widgets.dll
             ${QT_BIN_PATH}/libGLESv2.dll
             ${QT_BIN_PATH}/libEGL.dll
             ${PROJECT_BINARY_DIR}/qt.conf
        DESTINATION bin
        COMPONENT applications)

    install(
        FILES
            ${QT_BIN_PATH}/../plugins/iconengines/qsvgicon.dll
        DESTINATION bin/plugins/iconengines
        COMPONENT applications)

    install(
        FILES
            ${QT_BIN_PATH}/../plugins/imageformats/qgif.dll
            ${QT_BIN_PATH}/../plugins/imageformats/qico.dll
            ${QT_BIN_PATH}/../plugins/imageformats/qjpeg.dll
            ${QT_BIN_PATH}/../plugins/imageformats/qmng.dll
            ${QT_BIN_PATH}/../plugins/imageformats/qsvg.dll
            ${QT_BIN_PATH}/../plugins/imageformats/qtga.dll
            ${QT_BIN_PATH}/../plugins/imageformats/qtiff.dll
            ${QT_BIN_PATH}/../plugins/imageformats/qwbmp.dll
        DESTINATION bin/plugins/imageformats
        COMPONENT applications)

    install(
        FILES
            ${QT_BIN_PATH}/../plugins/mediaservice/dsengine.dll
        DESTINATION bin/plugins/mediaservice
        COMPONENT applications)

    install(
        FILES
            ${QT_BIN_PATH}/../plugins/platforms/qminimal.dll
            ${QT_BIN_PATH}/../plugins/platforms/qoffscreen.dll
            ${QT_BIN_PATH}/../plugins/platforms/qwindows.dll
        DESTINATION bin/plugins/platforms
        COMPONENT applications)


    install(
        FILES
            ${QT_BIN_PATH}/../plugins/printsupport/windowsprintersupport.dll
        DESTINATION bin/plugins/printsupport
        COMPONENT applications)


    install(
        FILES
            "${PROJECT_BINARY_DIR}/graphviz-2.36.msi"
        DESTINATION tmp
        COMPONENT applications)

#    list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
#        ExecWait '$INSTDIR\\\\tmp\\\\graphviz-2.36.msi'
#        ")

    list(APPEND CPACK_COMPONENT_EXTRA_INSTALL_COMMANDS "
        ExecWait 'msiexec /i $INSTDIR\\\\tmp\\\\graphviz-2.36.msi'
        ")
    list(APPEND CPACK_COMPONENT_EXTRA_UNINSTALL_COMMANDS "
        ExecWait 'msiexec /u $INSTDIR\\\\tmp\\\\graphviz-2.36.msi'
        ")

ELSE(WIN32 AND NOT UNIX)
    #SET(CPACK_STRIP_FILES "bin/dot-ed")
    #SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)

set(CPACK_PACKAGE_NAME "dot-editor")
set(CPACK_PACKAGE_VENDOR "Nicu Tofan")
SET(CPACK_PACKAGE_EXECUTABLES "dot-ed" "Dot Editor")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Dot Editor acts as a graphical interface for the popular graphviz package")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION ${DOTEDITOR_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${DOTEDITOR_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${DOTEDITOR_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${DOTEDITOR_PATCH_VERSION})
# set(CPACK_PACKAGE_INSTALL_DIRECTORY "CPack Component Example")
# SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
    "Includes dot-ed program and dot-editor library")
set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION
    "Static libraries used to build programs with dot-editor")
set(CPACK_COMPONENT_HEADERS_DESCRIPTION
    "C/C++ header files for use with dot-editor")
set(CPACK_COMPONENT_HEADERS_DEPENDS libraries)

set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
set(CPACK_COMPONENT_LIBRARIES_GROUP "Development")
set(CPACK_COMPONENT_HEADERS_GROUP "Development")
set(CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION
    "Tools that are needed to build plug-ins for Dot Editor")
set(CPACK_COMPONENT_GROUP_RUNTIME_DESCRIPTION
    "Applications contained in this package")


INCLUDE(CPack)
# ============================================================================

message (${CMAKE_SYSTEM})
