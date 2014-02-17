/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			settings.h
  \date			Mar 2012
  \author		TNick

  \brief		Contains the definition for Settings class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __SETTINGS_INC__
#define __SETTINGS_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <QDialog>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

class	QModelIndex;

namespace Ui {
class Settings;
}


namespace	Gui		{

class Settings : public QDialog
{
	Q_OBJECT

private:

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
	*	@brief	content generated from form
	*/
	Ui::Settings *		ui;


	/**
	*	@brief	tell if the snippets were edited
	*/
	bool				b_snipp_ed_;


	/**
	*	@brief	the one and only instance if shown, otherwise NULL
	*/
	static Settings *	uniq_;

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
	explicit Settings			( QWidget *parent = 0 );


	/**
	*	@brief	destructor
	*/
	~Settings();




protected:


	/**
	*	@brief	Implemented to allow trapping the language change event
	*/
	void				changeEvent		( QEvent *e );


	/**
	*	@brief	Implemented to allow saing the changes before exit
	*/
	void				closeEvent		( QCloseEvent *event );




private slots:


	/**
	*	@brief	applies the settings to main window
	*/
	void				saveStgToMain	( void );


	/**
	*	@brief	shows default values in dialog
	*/
	void				resetToDef		( void );


	/**
	*	@brief	shows values from man window
	*/
	void				resetToMain		( void );


	/**
	*	@brief	select the program to run
	*/
	void				selProgram		( void );


	/**
	*	@brief	select the database to use
	*/
	void				selSnipDbf		( void );


	/**
	*	@brief	load new database in the editor
	*/
	void				changeSnipDbf	( void );


	/**
	*	@brief	informed about changes in snippets editor
	*/
	void				txSnipChanged	( void );


	/**
	*	@brief	double-click in list of plug-ins
	*/
	void				dbClickPlugIns	(
			const QModelIndex &			mi
			);




private:


	/**
	*	@brief	load the list of plug-ins from plug-in manager
	*
	*	The method clears current content in the list and requests fresh info.
	*/
	void				getPlugInList	( void );


	/**
	*	@brief	ask the PlugIn Manager to syncronise its state
	*/
	void				savePlugInList	( void );




public:


	/**
	*	@brief	present the settings dialog if not shown already
	*/
	static void			showSetting		( void );


/*  FUNCTIONS    ======================================================= */
//
//
//
//

};	/*	class MW	*/

/*  CLASS    =============================================================== */
//
//
//
//

}	//	namespace	Gui

#endif // __MW_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
