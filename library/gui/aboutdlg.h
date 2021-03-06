/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			aboutdlg.h
  \date			Mar 2012
  \author		TNick

  \brief		Contains the definition for AboutDlg class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __ABOUTDLG_INC__
#define __ABOUTDLG_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include    <QDialog>

#include	"ui_aboutdlg.h"

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

namespace	Gui		{

/**
*	@brief	Dialog to briefly present the program
*/
class AboutDlg : public QDialog		{
	Q_OBJECT
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

	/**
	*	@brief	GUI components embedded in this instance
	*/
	Ui::AboutDlg 			ui;


	/*  DATA    ============================================================ */
	//
	//
	//
	//
	/*  FUNCTIONS    ------------------------------------------------------- */

public:

	/**
	*	@brief	constructor
	*/
	explicit			AboutDlg		 (QWidget *parent = 0);

	/**
	*	@brief	destructor
	*/
	~AboutDlg			();

protected:
	void				changeEvent		 (QEvent *e);

	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class AboutDlg	*/

/*  CLASS    =============================================================== */
//
//
//
//

}	//	namespace	Gui

#endif // __ABOUTDLG_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
