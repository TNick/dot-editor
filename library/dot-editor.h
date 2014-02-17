/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dot-editor.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef DOTEDITOR_main_h_INCLUDE
#define DOTEDITOR_main_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include    <dot-editor/config.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! describes the result of a function
enum	OUTCOME			{
    OUTCOME_WARNING = -1,
    OUTCOME_OK = 0,
    OUTCOME_ERROR = 1
};

//! was the result a succes (includes warnings)?
#define		OUT_SUCCESS( o )	( o != OUTCOME_ERROR )

//! was the result a succes (excludes warnings)?
#define		OUT_FULL_SUCCESS( o )	( o == OUTCOME_OK )

//! prepare the library for run
DE_EXPORT bool
dot_editor_lib_init ();

//! terminate the library
DE_EXPORT void
dot_editor_end ();

//! show main gui
DE_EXPORT void
dot_editor_lib_show_main_gui (const QString & init_file);

/*  DEFINITIONS    ========================================================= */
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

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#endif /* DOTEDITOR_main_h_INCLUDE */
