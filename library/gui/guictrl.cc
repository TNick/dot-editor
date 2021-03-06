/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			guictrl.cc
  \date			May 2012
  \author		TNick

  \brief		Contains the implementation of GuiCtrl class


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

#include	<dot-editor.h>

#include	<gui/mw.h>
#include	"guictrl.h"


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

using namespace Gui;

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
QWidget *		GuiCtrl::mainGui				()
{
	return Gui::MW::unique();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString 		GuiCtrl::editorText		()
{
	Gui::MW * mw = Gui::MW::unique();
	if (mw == NULL)
		return QString();
	return mw->editorText();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QGraphicsView *	GuiCtrl::graphicWidget	()
{
	Gui::MW * mw = Gui::MW::unique();
	if (mw == NULL)
		return NULL;
	return mw->graphicWidget();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			GuiCtrl::setImplem	 (ImplInterf * new_impl)
{
	Gui::MW * mw = Gui::MW::unique();
	if (mw == NULL)
		return;
	mw->setImplem (new_impl);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ImplInterf *	GuiCtrl::implem			()
{
	Gui::MW * mw = Gui::MW::unique();
	if (mw == NULL)
		return NULL;
	return mw->implem();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QDockWidget *	GuiCtrl::addDock	 (const QString & s_name)
{
	Gui::MW * mw = Gui::MW::unique();
	if (mw == NULL)
		return NULL;
	QDockWidget * dk;
	dk = new QDockWidget (s_name, mw);
	dk->setObjectName (s_name);
	mw->addDockWidget (Qt::RightDockWidgetArea, dk);
	mw->addMenuViewAction (dk->toggleViewAction());

	return dk;
}
/* ========================================================================= */

/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
