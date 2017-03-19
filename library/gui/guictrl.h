/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			guictrl.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for GuiCtrl class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __GUICTRL_INC__
#define __GUICTRL_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<QString>

class	QWidget;
class	QGraphicsView;
class	QDockWidget;
class	ImplInterf;

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/**
*	@brief	Provides a way of accesing Gui functionality
*
*	All the methods in the class are static.
*/
class DE_EXPORT	GuiCtrl				{

	//
	//
	//
	//
	/*  DEFINITIONS    ----------------------------------------------------- */

	/*  DEFINITIONS    ===================================================== */
	//
	//
	//
	//
	/*  DATA    ------------------------------------------------------------ */

private:



	/*  DATA    ============================================================ */
	//
	//
	//
	//
	/*  FUNCTIONS    ------------------------------------------------------- */

public:

	/**
	*	@brief	the text in application's editor
	*/
	static QWidget *				mainGui				 ();

	/**
	*	@brief	the text in application's editor
	*/
	static QString					editorText			 ();


	/**
	*	@brief	the graphics display
	*/
	static QGraphicsView *			graphicWidget		 ();


	/**
	*	@brief	change current implementation
	*/
	static void						setImplem			(
			ImplInterf *				new_impl
			);


	/**
	*	@brief	get current implementation
	*/
	static ImplInterf *				implem			 ();


	/**
	*	@brief	create a tool window in dock panel
	*
	*	The function also creates an associated action.
	*/
	static QDockWidget *			addDock			(
			const QString &				s_name
			);


	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class GuiCtrl	*/

/*  CLASS    =============================================================== */
//
//
//
//
#endif // __GUICTRL_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
