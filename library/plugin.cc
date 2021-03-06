/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plugin.cc
  \date			May 2012
  \author		TNick

  \brief		Contains the implementation for PlugIn class


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

#include	<QToolBar>

#include	<plginman.h>

#include	<gui/mw.h>

#include	"plugin.h"

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

using namespace PlugIns;

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
PlugIn::PlugIn	()
{
	/* append to the list of loaded plug-ins */
	PlgInMan::append (this);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
PlugIn::~PlugIn	()
{
	/* extract from the list of loaded plug-ins */
	PlgInMan::remove (this);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool		PlugIn::initComplete (UserMsg & um)
{
	Q_UNUSED (um);
	return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void		PlugIn::systemClosing (UserMsg & um)
{
	Q_UNUSED (um);
	delete this;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QToolBar *	PlugIn::createTb		(
		Qt::ToolBarArea tb_a, const QString & s_name )
{
	QToolBar * tb =	new QToolBar (s_name);
	Gui::MW::unique()->addToolBar (tb_a, tb);
	return tb;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void		PlugIn::connectTo_TxChanged	(
		QObject *dest_obj, const char *dest_slt )
{
	Q_ASSERT (Gui::MW::unique() != NULL);
	Gui::MW::unique()->editorTxChanged (dest_obj, dest_slt);
}
/* ========================================================================= */

/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
