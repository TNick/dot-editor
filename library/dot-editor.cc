/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dot-editor.cc
  \date			Jul 2011
  \author		TNick

  \brief		General routines for the library


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<dot-editor/dot-editor.h>
#include	<dot-editor/usermsg.h>
#include	<dot-editor/plginman.h>
#include	<dot-editor/gui/mw.h>

#include    <QDebug>
#include    <QSettings>
#include    <QTranslator>
#include    <QDir>
#include    <QDesktopServices>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

bool dot_editor_lib_init ()
{

    Q_INIT_RESOURCE(application);

    UserMsg::startUp();

    /* application is translatable */
    QString locale = QSettings().value(
                STG_LANGUAGE,
                QLocale::system().name()
                ).toString();
    QTranslator translator;
    QDir	d(
            #if QT_VERSION < 0x050000
                        QDesktopServices::storageLocation(
                            QDesktopServices::TempLocation )
            #else
                        QStandardPaths::standardLocations (
                            QStandardPaths::DataLocation).at(0)
            #endif
                );
    d.cd( DATA_FLD_TRANSL );
    translator.load(
                d.absoluteFilePath(
                    LANG_FILE( locale ) ) );
    qApp->installTranslator( &translator );

    return true;
}

void dot_editor_end ()
{
    UserMsg::finishUp();
}

void dot_editor_lib_show_main_gui (const QString & init_file)
{

    /* show GUI */
    Gui::MW * main_win = new Gui::MW (NULL, init_file);
#	if defined(Q_OS_SYMBIAN)
    main_win->showMaximized();
#	else
    main_win->show();
#	endif

    /* enable communication with the user */
    UserMsg::initialDisplay();

    /* prepare for plug-ins */
    PlugIns::PlgInMan::init();

}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
