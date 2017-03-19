/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			usermsggui.cc
  \date			Dec 2011
  \author		TNick

  \brief		Contains the implementation for UserMsgGui class


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

#include	<QDebug>

#include	<gui/mw.h>

#include	"usermsggui.h"

using namespace Gui;

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

extern void		GUI_showPopUpMessage	 (UserMsg::MsgCacheList * p_msg);
extern void		GUI_showPopUpMessage	 (UserMsg * p_msg);

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

/* only one instance; if shown we use that */
UserMsgGui *		UserMsgGui::uniq_inst_ = NULL;


/*  DATA    ================================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
UserMsgGui::UserMsgGui					(QWidget *parent) :
	QDialog(parent)
{
	/* only one instance is allowed at a time */
	Q_ASSERT(uniq_inst_ == NULL);
	uniq_inst_ = this;

	/* prepare controls */
	ui.setupUi(this);

	ui.ck_displErr->setChecked(UserMsg::isTypeVisible(UserMsg::MSG_ERROR));
	ui.ck_displWar->setChecked(UserMsg::isTypeVisible(UserMsg::MSG_WARNING));
	ui.ck_displInfo->setChecked(UserMsg::isTypeVisible(UserMsg::MSG_INFO));
	ui.ck_displDebug->setChecked(
				UserMsg::isTypeVisible(UserMsg::MSG_DBG_ERROR) |
				UserMsg::isTypeVisible(UserMsg::MSG_DBG_WARNING) |
				UserMsg::isTypeVisible(UserMsg::MSG_DBG_INFO)
				);

	connect(ui.b_OK, SIGNAL(clicked()), this, SLOT(closeMe()));
	connect(ui.b_Help, SIGNAL(clicked()), this, SLOT(help()));
	connect(ui.b_Settings, SIGNAL(clicked()), this, SLOT(showSettings()));
	connect(ui.ck_displErr, SIGNAL(clicked()), this, SLOT(chgWhatIsShown()));
	connect(ui.ck_displWar, SIGNAL(clicked()), this, SLOT(chgWhatIsShown()));
	connect(ui.ck_displInfo, SIGNAL(clicked()), this, SLOT(chgWhatIsShown()));
	connect(ui.ck_displDebug, SIGNAL(clicked()), this, SLOT(chgWhatIsShown()));



	show();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
UserMsgGui::~UserMsgGui					()
{
	Q_ASSERT(uniq_inst_ == this);
	uniq_inst_ = NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void					UserMsgGui::changeEvent			(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:	{
		ui.retranslateUi(this);
		break;}
	default:						{
		break;}
	}
}
/* ========================================================================= */



/* ------------------------------------------------------------------------- */
void					UserMsgGui::showMsg			(UserMsg * msg)
{
	if (!isShown())
	{
		new UserMsgGui (MW::unique());
	}

	UserMsg::MsgCacheEntry new_e;
	msg->exportData (new_e);

	uniq_inst_->m_lst_.append (new_e);
	uniq_inst_->updateText();

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void					UserMsgGui::showMsg			(UserMsg::MsgCacheList * msg)
{
	if (!isShown())
	{
		new UserMsgGui (MW::unique());
	}

	for (int i = 0; i < msg->count(); i++)
	{
		uniq_inst_->m_lst_.append (msg->at(i));
	}

	uniq_inst_->updateText();
}
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
void					UserMsgGui::updateText		()
{
	uniq_inst_->ui.txArea->clear();
	QString	s_out = UserMsg::cacheListToHtml (m_lst_);
	if (s_out.isEmpty())
	{
		closeMe();
	}
	uniq_inst_->ui.txArea->setHtml(
				uniq_inst_->ui.txArea->toHtml().replace(
					"</body></html>",
					s_out + "</body></html>"
					)
				 );
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void					UserMsgGui::closeMe				()
{
	close();
	deleteLater();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void					UserMsgGui::help				()
{
	/// @todo implement help
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void					UserMsgGui::showSettings		()
{
	/// @todo open settings at proper tab

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void					UserMsgGui::chgWhatIsShown		()
{
	int	flg = 0;

	if (ui.ck_displErr->isChecked())
	{
		flg = flg | UserMsg::MVIZ_ERROR;
	}
	if (ui.ck_displWar->isChecked())
	{
		flg = flg | UserMsg::MVIZ_WARNING;
	}
	if (ui.ck_displInfo->isChecked())
	{
		flg = flg | UserMsg::MVIZ_INFO;
	}
	if (ui.ck_displDebug->isChecked())
	{
		flg = flg | UserMsg::MVIZ_DBG_ERROR
				| UserMsg::MVIZ_DBG_WARNING
				| UserMsg::MVIZ_DBG_INFO;
	}

	UserMsg::setTypeVisible (static_cast<UserMsg::MsgVisible>( flg ));

	updateText();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void					UserMsgGui::maskChanged		()
{
	if (uniq_inst_ != NULL)
	{
		uniq_inst_->ui.ck_displErr->setChecked(UserMsg::isTypeVisible(UserMsg::MSG_ERROR));
		uniq_inst_->ui.ck_displWar->setChecked(UserMsg::isTypeVisible(UserMsg::MSG_WARNING));
		uniq_inst_->ui.ck_displInfo->setChecked(UserMsg::isTypeVisible(UserMsg::MSG_INFO));
		uniq_inst_->ui.ck_displDebug->setChecked(
					UserMsg::isTypeVisible(UserMsg::MSG_DBG_ERROR) |
					UserMsg::isTypeVisible(UserMsg::MSG_DBG_WARNING) |
					UserMsg::isTypeVisible(UserMsg::MSG_DBG_INFO)
					);

		uniq_inst_->updateText();
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void		GUI_showPopUpMessage (UserMsg::MsgCacheList * p_msg)
{
	UserMsgGui::showMsg (p_msg);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void		GUI_showPopUpMessage (UserMsg * p_msg)
{
	UserMsgGui::showMsg (p_msg);
}
/* ========================================================================= */


/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
