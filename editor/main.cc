/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			main.cc
  \date			February 2014
  \author		TNick

  \brief		Contains application's entry point


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

#include    <QDebug>
#include    <QSettings>
#include    <QTranslator>
#include    <QDir>
#include    <QDesktopServices>
#include    <QApplication>

#ifdef TARGET_SYSTEM_WIN32
#include <windows.h>
#endif

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

/* ------------------------------------------------------------------------- */
void		printUsage			( void )
{
    qDebug()
            << "Usage:\n"
            << "dot-ed\n"
            << QObject::tr( "  to start the program with a new file" ) << "\n\n"
            << "dot-ed file\n"
            << QObject::tr( "  to start the program and load the file" ) << "\n\n"
               ;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool		parseArgs			(
        int argc, char *argv[], int * ret_val, QString & s_init_file )
{

    int b = 0;

    for ( int i = 1; i < argc; i++ ) {

        b = 0;
        if ( argv[i][0] == '-' ) {
            b = 1;
            if ( argv[i][1] == '-' )
            {
                b = 2;
            }
            qDebug() << QString( QObject::tr(
                                     "ERROR! Argument %1 was not understood." ) )
                        .arg( QString( argv[i]+b ) )
                     << "\n\n";
            printUsage();
            *ret_val = -1;
            return false;
        } else {
            if ( s_init_file.isEmpty() ) {
                s_init_file = argv[i];
            } else {
                qDebug() << QObject::tr( "ERROR! Only one file should be provided as argument." )
                         << "\n\n";
                printUsage();
                *ret_val = -1;
                return false;
            }
        }
    }

    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#if defined(DOTEDITOR_WIN32)
int WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
    int argc = __argc;
    char **argv = __argv;
#else
int   main    ( int argc, char *argv[] )
{
#endif

    int ret_val = 0;

    try
    {

        /* prepare to start */
        QApplication apl( argc, argv );

        /* prepare the ground for easy access to settings */
        QCoreApplication::setOrganizationName( "TNick" );
        QCoreApplication::setOrganizationDomain( "sites.google.com/site/doteditorhome/" );
        QCoreApplication::setApplicationName( "dot-ed" );

        // prepare the library
        if (dot_editor_lib_init() == false) {
            qDebug() << QObject::tr( "ERROR! Failed to initialize dot-editor library." )
                     << "\n\n";
            return 1;
        }

        // parse arguments; may exit
        QString s_init_file;
        if (parseArgs (argc, argv, &ret_val, s_init_file) == false) {
            return ret_val;
        }

        // start the main gui
        dot_editor_lib_show_main_gui (s_init_file);

        // exit
        ret_val = apl.exec();

        // terminate the library
        dot_editor_end();

        return ret_val;

    }
    catch (...) {  }
    return -1;

}
/* ========================================================================= */


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
