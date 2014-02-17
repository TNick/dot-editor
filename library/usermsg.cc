/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!				\file		usermsg.cc
 \date		june 2010
 \author		TNick

 \brief		Implements UserMsg class


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

#include	<QFile>
#include	<QMessageBox>
#include	<QTime>
#include	<QDebug>
#include	<QSettings>

#include	"usermsg.h"

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

/* Intrface with the GUI for UserMsg */
extern bool		GUI_isCmdLogVisible		( void );
extern void		GUI_showCmdLog			( void );
extern void		GUI_showCmdLogMessage	( UserMsg::MsgCacheList * p_msg );
extern void		GUI_showCmdLogMessage	( UserMsg * p_msg );
extern void		GUI_showPopUpMessage	( UserMsg::MsgCacheList * p_msg );
extern void		GUI_showPopUpMessage	( UserMsg * p_msg );

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */


UserMsg::StatData *			UserMsg::d_ = NULL;


/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
void			UserMsg::startUp			( void )
{
	QSettings	stg;
	d_ = new StatData();
	d_->ty_names_ << QObject::tr("Error"),
			QObject::tr("Warning"),
			QObject::tr("Info"),
			QObject::tr("Debug Error"),
			QObject::tr("Debug Warning"),
			QObject::tr("Debug Info");

	/* start logging dead messages */
	if ( stg.value("stg/logDeadmsg").toBool() )
	{
		initDeadLog();
	}

	setOutputType( MSGTY_DISABLED );
	doDefaultFormat();

	bool ok = false;
	int	i_val = stg.value(
				"stg/msgFilter",
				MVIZ_ERROR | MVIZ_WARNING
				).toInt( &ok );
	if ( ok && (i_val >= 0) && (i_val < MVIZ_MAX) )
	{
		d_->viz_lst_ = static_cast<MsgVisible>( i_val );
	}
	else
	{
		d_->viz_lst_ = static_cast<MsgVisible>( MVIZ_ERROR | MVIZ_WARNING );
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			UserMsg::finishUp			( void )
{

	/* show any messages left in cache */
	setOutputType(MSGTY_POPUP);
	flush();
	setOutputType(MSGTY_DISABLED);

	/* if there is log file for dead messages, close it */
	if (d_->file_dead.isOpen())
	{
		d_->stx_dead.flush();
		d_->file_dead.close();
	}

	delete d_;
	d_ = NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			UserMsg::initialDisplay			( void )
{
	QSettings	stg;
	bool ok = false;
	int	i_val = stg.value( "stg/msgType", MSGTY_LOG_OR_POPUP ).toInt( &ok );
	if ( ok && (i_val >= 0) && (i_val < MSGTY_MAX) )
	{
		UserMsg::setOutputType( static_cast<MsgOutType>( i_val ) );
	}
	else
	{
		UserMsg::setOutputType( MSGTY_LOG_OR_POPUP );
	}

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			UserMsg::initDeadLog		( void )
{
	QSettings	stg;
	if (d_->file_dead.isOpen())
	{
		d_->stx_dead.flush();
		d_->file_dead.close();
	}

	QString s_file = stg.value("stg/logDeadmsg").toString();
	if ( s_file.isNull() == false )
	{
		d_->file_dead.setFileName( s_file );
		if ( d_->file_dead.open( QFile::WriteOnly ) )
		{
			d_->stx_dead.setDevice( &d_->file_dead );
			return;
		}
	}

	QMessageBox::warning(NULL,
                         DOTEDITOR_PROJECT_NAME,
						 QObject::tr("Dead messages will not be \n"
									 "logged because the file \n"
									 "%1\n"
									 "could not be opened")
						 .arg(s_file));
	d_->file_dead.open(stdout, QIODevice::WriteOnly);
	d_->stx_dead.setDevice( &d_->file_dead );

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void					UserMsg::doDefaultFormat		( bool b_forced )
{
	QSettings	stg;

	QString	s_tmp;
	d_->html_format_.clear();

	s_tmp = stg.value( "stg/ERROR_FORMAT" ).toString();
	if ( ( b_forced || s_tmp.isEmpty() ) )
	{
		s_tmp = "<p><font color=\"red\">Error: ##TEXT##</font></p>\n";
		stg.setValue( "stg/ERROR_FORMAT", s_tmp );
	}
	d_->html_format_ << s_tmp;

	s_tmp = stg.value( "stg/WARNING_FORMAT" ).toString();
	if ( ( b_forced || s_tmp.isEmpty() ) )
	{
		s_tmp = "<p><font color=\"green\">Warning: ##TEXT##</font></p>\n";
		stg.setValue( "stg/WARNING_FORMAT", s_tmp );
	}
	d_->html_format_ << s_tmp;

	s_tmp = stg.value( "stg/INFO_FORMAT" ).toString();
	if ( ( b_forced || s_tmp.isEmpty() ) )
	{
		s_tmp = "<p><font color=\"blue\">Information: ##TEXT##</font></p>\n";
		stg.setValue( "stg/INFO_FORMAT", s_tmp );
	}
	d_->html_format_ << s_tmp;

	s_tmp = stg.value( "stg/DBG_ERROR_FORMAT" ).toString();
	if ( ( b_forced || s_tmp.isEmpty() ) )
	{
		s_tmp = "<p><font color=\"red\">Debug Error: ##TEXT##</font></p>\n";
		stg.setValue( "stg/DBG_ERROR_FORMAT", s_tmp );
	}
	d_->html_format_ << s_tmp;

	s_tmp = stg.value( "stg/DBG_WARNING_FORMAT" ).toString();
	if ( ( b_forced || s_tmp.isEmpty() ) )
	{
		s_tmp = "<p><font color=\"red\">Debug warning: ##TEXT##</font></p>\n";
		stg.setValue( "stg/DBG_WARNING_FORMAT", s_tmp );
	}
	d_->html_format_ << s_tmp;

	s_tmp = stg.value( "stg/DBG_INFO_FORMAT" ).toString();
	if ( ( b_forced || s_tmp.isEmpty() ) )
	{
		s_tmp = "<p><font color=\"red\">Debug information: ##TEXT##</font></p>\n";
		stg.setValue( "stg/DBG_INFO_FORMAT", s_tmp );
	}
	d_->html_format_ << s_tmp;

	s_tmp = stg.value( "stg/FORMAT" ).toString();
	if ( ( b_forced || s_tmp.isEmpty() ) )
	{
		s_tmp = "\n<h3>Message ##MSGID## at ##TIME##</h3>\n##MESSAGES##\n<br><br>";
		stg.setValue( "stg/FORMAT", s_tmp );
	}
	d_->html_format_ << s_tmp;

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			UserMsg::setOutputType		( MsgOutType new_ty )
{
	bool	b_flush = false;

	if ( d_->out_ty_ == new_ty )
		return;

	if ( d_->out_ty_ == MSGTY_DISABLED )
	{
		b_flush = true;
	}

	d_->out_ty_ = new_ty;

	if (b_flush)
	{
		flush();
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			UserMsg::flush				( void )
{

	if ( d_->out_ty_ == MSGTY_DISABLED )
	{
		return;
	}

	if (d_->cache_msg_.count() > 0)
	{
		bool	in_log;

		switch	( d_->out_ty_ )		{

		case	MSGTY_POPUP:			{
			in_log = false;
			break; }

		case	MSGTY_LOG_OR_POPUP:		{
			in_log = GUI_isCmdLogVisible( );
			break; }

		case	MSGTY_LOG_FORCED:		{
			if ( !GUI_isCmdLogVisible( ) )
				GUI_showCmdLog();
			in_log = true;
			break; }

		case	MSGTY_LOG:				{
			in_log = true;
			break; }

		default:						{
			Q_ASSERT( 0 );
			in_log = false;
			break; }
		}

		if ( in_log )
		{ /* in log window */
			GUI_showCmdLogMessage( &d_->cache_msg_ );
		}
		else
		{ /* in pop-up */
			GUI_showPopUpMessage( &d_->cache_msg_ );
		}

		d_->cache_msg_.clear();
	}

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
UserMsg::UserMsg		( )
{

	/* provides an unique ID for each instance */
	id_ = d_->ID_factory; d_->ID_factory++;

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
UserMsg::~UserMsg		( )
{
	QSettings	stg;

	/* see if there are messages left unshown */
	if ( msg_.count( ) != 0 )
	{
		if ( stg.value("stg/logDeadmsg").toBool() )
		{
			if ( d_->stx_dead.device() == NULL )
			{
				initDeadLog();
			}

			d_->stx_dead << "<message id=" << id_ << ">\n";
			d_->stx_dead << "   <time>" << QTime::currentTime().toString() << "</time>";

			for (int i = 0; i < msg_.count( ); i++)
			{
				d_->stx_dead << "   <entry type=\""
						 << tyName(msg_.at(i).type)
						 << "\">"
						 << msg_.at(i).text
						 << "</entry>";
			}
			d_->stx_dead << "</message>\n";
		}
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			UserMsg::add				( MsgType ty, QString message )
{
	MsgEntry	new_e;
	new_e.text = message;
	new_e.type = ty;
	msg_.append( new_e );
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			UserMsg::show				( void )
{
	if ( msg_.count() == 0 )
	{
		return;
	}

	if ( d_->out_ty_ == MSGTY_DISABLED )
	{
		MsgCacheEntry	ce;
		ce.id_ = id_;
		ce.time_ = QTime::currentTime();
		ce.msg_ = msg_;
		d_->cache_msg_.append(ce);
	}
	else
	{
		bool	in_log;
		switch	( d_->out_ty_ )		{

		case	MSGTY_POPUP:			{
			in_log = false;
			break; }

		case	MSGTY_LOG_OR_POPUP:		{
			in_log = GUI_isCmdLogVisible( );
			break; }

		case	MSGTY_LOG_FORCED:		{
			if ( !GUI_isCmdLogVisible( ) )
				GUI_showCmdLog();
			in_log = true;
			break; }

		case	MSGTY_LOG:				{
			in_log = true;
			break; }

		default:						{
			Q_ASSERT( 0 );
			in_log = false;
			break; }
		}

		if ( in_log )
		{
			GUI_showCmdLogMessage( this );
		}
		else
		{
			GUI_showPopUpMessage( this );
		}

	}

	/* prepare the structure for a new message */
	msg_.clear();
	id_ = d_->ID_factory; d_->ID_factory++;

	return;

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			UserMsg::exportData			( MsgCacheEntry & outp )
{
	outp.id_ = id_;
	outp.msg_ = msg_;
	outp.time_ = QTime::currentTime();
}
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
void			UserMsg::setTypeVisible			( MsgType ty, bool b_viz )
{
	QSettings	stg;
	if ( (ty >= 0) && (ty < MSG_MAX_TY) )
	{
		int flg = (1 << ty);
		if (b_viz)
		{
			d_->viz_lst_ = static_cast<MsgVisible>( d_->viz_lst_ | flg );
		}
		else
		{
			d_->viz_lst_ = static_cast<MsgVisible>( d_->viz_lst_ & (~flg) );
		}
		stg.setValue( "stg/msgFilter", d_->viz_lst_ );

	}

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			UserMsg::setTypeVisible			( MsgVisible ty )
{
	QSettings	stg;
	d_->viz_lst_ = static_cast<MsgVisible>( ty & MVIZ_MAX );
	stg.setValue( "stg/msgFilter", d_->viz_lst_ );
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString			UserMsg::cacheListToHtml		( const MsgCacheList & c_list )
{

	QString		ret;
	for ( int i = 0; i < c_list.count(); i++ )
	{
		ret += cacheEntryToHtml( c_list.at( i ) );
	}
	return ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString			UserMsg::cacheEntryToHtml		( const MsgCacheEntry & c_entry )
{
	QString		messages = msgListToHtml( c_entry.msg_ );
	QString		ret;
	if (!messages.isEmpty())
	{
		ret = d_->html_format_[MGT_MESSAGE];
		return ret
				.replace( "##MSGID##", QString::number( c_entry.id_) )
				.replace( "##TIME##", c_entry.time_.toString() )
				.replace( "##MESSAGES##", messages );
	}

	return QString();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString			UserMsg::toHtml		( void )
{
	QString		messages = msgListToHtml( msg_ );
	QString		ret;
	if (!messages.isEmpty())
	{
		ret = d_->html_format_[MGT_MESSAGE];
		return ret
				.replace( "##MSGID##", QString::number( id_) )
				.replace( "##TIME##", QTime::currentTime().toString() )
				.replace( "##MESSAGES##", messages );
	}

	return QString();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString			UserMsg::msgListToHtml			( const MsgList & m_list )
{
	QString		ret;

	for ( int j = 0; j < m_list.count(); j++ )
	{
		ret += msgEntryToHtml( m_list.at( j ) );
	}

	return ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString			UserMsg::msgEntryToHtml			( const MsgEntry & m_entry )
{
	if (isTypeVisible(m_entry.type))
	{
		QString t1 = m_entry.text;
		t1.replace("<", "&lt;").replace(">", "&gt;");
		t1.replace("\n", "<br>");
		QString t2 = d_->html_format_[m_entry.type];
		return t2.replace("##TEXT##", t1);
	}

	return QString();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			UserMsg::fatalException			( void )
{
	QMessageBox::warning( NULL,
                         DOTEDITOR_PROJECT_NAME,
						 QObject::tr(
							"Fatal exception. "
							"The application can't continue." ) );
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
