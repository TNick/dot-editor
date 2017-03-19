/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			usermsggui.h
  \date			Dec 2011
  \author		TNick

  \brief		Contains the definition for UserMsgGui class


*//*

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __USERMSGGUI_INC__
#define __USERMSGGUI_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<dot-editor.h>

#include    <QDialog>
#include    <QStringList>

#include	<usermsg.h>

#include	"ui_usermsggui.h"

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */


namespace	Gui		{

/// form that presents the messages to the user
/**
*
*/
class UserMsgGui : public QDialog		{
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

	/// GUI components
	Ui::UserMsgGui					ui;

	/// list of currently displayed things
	UserMsg::MsgCacheList			m_lst_;



	/// only one instance; if shown we use that
	static UserMsgGui *				uniq_inst_;


	/*  DATA    ============================================================ */
	//
	//
	//
	//
	/*  FUNCTIONS    ------------------------------------------------------- */

private:

	/// constructor;
	explicit			UserMsgGui		(QWidget *parent = 0);

	/// destructor;
	~UserMsgGui			 ();

	/// updates the text shown to the user based on the \b m_lst_
	void				updateText			 ();


public:

	/// show a message block
	static void			showMsg			(UserMsg * msg);

	/// show a group of message blocks
	static void			showMsg			(UserMsg::MsgCacheList * msg);

	/// tell if the dialog is shown or not
	static bool			isShown			()
	{ return uniq_inst_ != NULL; }

	/// informed that it should update the display (check boxes and text)
	static void			maskChanged		 ();

protected:

	/// needed for dynamic language change
	void				changeEvent		(QEvent *e);

protected slots:
	void				closeMe					 ();
	void				help					 ();
	void				showSettings			 ();
	void				chgWhatIsShown			 ();


	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class UserMsgGui	*/

/*  CLASS    =============================================================== */
//
//
//
//

}	//	namespace	Gui

#endif // __USERMSGGUI_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
