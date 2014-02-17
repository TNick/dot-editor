/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			enfautomata.cc
  \date			May 2012
  \author		TNick

  \brief		Contains the implementation of ENFAutomata class


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

#include	<dot-editor/plugins/automata/automatacomobj.h>
#include	<dot-editor/plugins/automata/de-plgin-automata.h>


#include	"enfautomata.h"


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */


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
/*  CLASS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
ENFAutomata::ENFAutomata	( PlugIns::Automata_PlugIn * plg_inst )
	: CmnAutomImpl( plg_inst )
{
	/* stub */
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ENFAutomata::~ENFAutomata	( void )
{
	/* stub */
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ENFAutomata::goStep				( const QString & s_act )
{
	Q_UNUSED( s_act );
	/**  @todo implement goStep */
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ENFAutomata::reset				( void )
{
	/**  @todo implement reset */
	/* stub */
}
/* ========================================================================= */

/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */