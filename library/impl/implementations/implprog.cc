/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			implprog.cc
  \date			May 2012
  \author		TNick

  \brief		Contains the implementation of ImplProg class


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

#include	<QFile>
#include	<QTemporaryFile>
#include	<QSettings>
#include	<QMenu>
#include	<QAction>
#include	<QGraphicsPixmapItem>
#include	<QFileDialog>
#include	<QDesktopServices>
#include	<QDebug>

#include	<usermsg.h>
#include	<gui/guictrl.h>
#include	<gui/mw.h>


#include	"implprog.h"


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


ImplProg *		ImplProg::uniq_ = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
ImplProg::ImplProg	( void )
	: ImplInterf()
{
	Q_ASSERT( uniq_ == NULL );
	uniq_ = this;
	dot_process = NULL;
	itm_img = NULL;

	s_.load();
	applySettings();

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ImplProg::~ImplProg	( void )
{

	if ( QFile::exists( cache_tmp_src_ ) )
	{
		QFile::remove( cache_tmp_src_ );
	}
	if ( QFile::exists( cache_tmp_dest_) )
	{
		QFile::remove( cache_tmp_dest_ );
	}

	s_.save();

	Q_ASSERT( uniq_ == this );
	uniq_ = NULL;

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::stopDotProcess	( void )
{
	if ( dot_process != NULL )
	{
		dot_process->terminate();
		dot_process->waitForFinished();
		delete dot_process;
		dot_process = NULL;
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::startDotProcess	( void )
{

	dot_process = new QProcess( qApp );
	connect( dot_process,SIGNAL(finished(int,QProcess::ExitStatus)),
			 this, SLOT(proc_finished(int,QProcess::ExitStatus))
			 );
	connect( dot_process,SIGNAL(started()),
			 this, SLOT(proc_started())
			 );
	connect( dot_process,SIGNAL(error(QProcess::ProcessError)),
			 this, SLOT(proc_error(QProcess::ProcessError))
			 );
	connect( dot_process,SIGNAL(readyReadStandardError()),
			 this, SLOT(proc_readyError())
			 );
	connect( dot_process,SIGNAL(readyReadStandardOutput()),
			 this, SLOT(proc_readyOutput())
			 );

	dot_process->start( "\"" + s_.s_process_ + "\" " + cache_s_arg_ );

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::inst_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED( exitCode );
	Q_UNUSED( exitStatus );

	QProcess * proc = static_cast<QProcess*>( sender() );
	proc->deleteLater();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::proc_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED( exitCode );
	Q_UNUSED( exitStatus );
	UserMsg	um;

	if ( dot_process != NULL )
	{
		dot_process->deleteLater();
		dot_process = NULL;
	}

	if ( itm_img != NULL )
	{
		Q_ASSERT( &img_scene == itm_img->scene() );
		img_scene.removeItem( itm_img );
		delete itm_img;
		itm_img = NULL;
	}

	itm_img = new QGraphicsPixmapItem(
				QPixmap( cache_tmp_dest_ ) ,
                NULL
				);
    img_scene.addItem (itm_img);

	if ( s_.b_log_start_end_ )
	{
		um.add( UserMsg::MSG_INFO,	QObject::tr(
					"Process ended." )
				);
		um.show();
	}
	img_scene.update();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::proc_started ()
{
	if ( s_.b_log_start_end_ )
	{
		UserMsg	um;
		um.add( UserMsg::MSG_INFO,	QObject::tr(
					"Process started." )
				);
		um.show();
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::proc_readyError ()
{
	QProcess * proc = static_cast<QProcess*>( sender() );

	if ( proc != NULL )
	{
		UserMsg	um;
		um.add( UserMsg::MSG_ERROR,	QObject::tr(
					proc->readAllStandardError() )
				);
		um.show();
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::proc_readyOutput ()
{
	QProcess * proc = static_cast<QProcess*>( sender() );

	if ( dot_process != NULL )
	{
		UserMsg	um;
		um.add( UserMsg::MSG_INFO,	QObject::tr(
					proc->readAllStandardOutput() )
				);
		um.show();
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::proc_error ( QProcess::ProcessError error )
{
	QProcess * proc = static_cast<QProcess*>( sender() );


	QString s_err;
	switch (error) {
	case QProcess::FailedToStart:	s_err = tr( "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program." ); break;
	case QProcess::Crashed:			s_err = tr( "The process crashed some time after starting successfully." ); break;
	case QProcess::Timedout:		s_err = tr( "The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again." ); break;
	case QProcess::WriteError:		s_err = tr( "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel." ); break;
	case QProcess::ReadError:		s_err = tr( "An error occurred when attempting to read from the process. For example, the process may not be running." ); break;
	case QProcess::UnknownError:	s_err = tr( "An unknown error occurred. This is the default return value of error()." ); break;
    default:						s_err = QString( "%1 error" ).arg( DOTEDITOR_PROJECT_NAME );
	}
	if ( proc != NULL )
	{
		proc->deleteLater();
		if ( proc == dot_process )
			dot_process = NULL;
	}

	UserMsg	um;
	um.add( UserMsg::MSG_ERROR,	s_err );
	um.show();

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool				ImplProg::perform			( void )
{

	stopDotProcess();

	QString s = GuiCtrl::editorText().trimmed();
	if ( s.isEmpty() )
	{
		/* remove image if one exists */
		if ( itm_img != NULL )
		{
			img_scene.removeItem( itm_img );
			delete itm_img;
			itm_img = NULL;
		}

		return true;
	}

	/* save the content to a file */
	if ( writeToTemp( s ) == false )
		return false;

	startDotProcess();

	return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::ctxDisplay		(
		const QPoint & loc, const QPoint & glob_loc )
{
	Q_UNUSED( loc );
	UserMsg		um;

	QMenu	mnu;
	QAction	ac_copy_img( tr( "Save image as..." ), &mnu );

	if ( itm_img != NULL )
	{
		ac_copy_img.setIcon( QIcon( ":/resources/images/save.png" ) );
		mnu.addAction( &ac_copy_img );
		connect( &ac_copy_img, SIGNAL( triggered() ),
				 this, SLOT( saveImageAs() ) );

		mnu.exec( glob_loc );
	}
	else
	{
		um.add( UserMsg::MSG_WARNING,	QObject::tr(
					"No image in preview panel." )
				);
		um.show();
	}

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QAction *			ImplProg::action					( void )
{
	return Gui::MW::unique()->predefAct_Prog();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::saveImageAs				( void )
{
	UserMsg		um;
	if ( itm_img != NULL )
	{
		QString fileName = QFileDialog::getSaveFileName( GuiCtrl::mainGui() );
		if (!fileName.isEmpty())
		{
			if ( itm_img->pixmap().save( fileName ) == false )
			{
				um.add( UserMsg::MSG_ERROR,	QObject::tr(
							"ERROR!!! The image could not be saved as \n%1" )
						.arg( fileName )
						);
				um.show();
			}
			else
			{
				um.add( UserMsg::MSG_ERROR,	QObject::tr(
							"The image was saved as\n%1" )
						.arg( fileName )
						);
				um.show();
			}
		}
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::applySettings		( void )
{
	QFile		f;
	UserMsg		um;

	/* remove previously used temporary files */
	if ( cache_tmp_dest_.isEmpty() == false )
	{
		f.setFileName( cache_tmp_dest_ );
		if ( f.exists() )
		{
			f.remove();
		}
	}
	if ( cache_tmp_src_.isEmpty() == false )
	{
		f.setFileName( cache_tmp_src_ );
		if ( f.exists() )
		{
			f.remove();
		}
	}

	/* get new temporary files */
    QString s_tmp =
#if QT_VERSION < 0x050000
			QDesktopServices::storageLocation(
				QDesktopServices::TempLocation );
#else
            QStandardPaths::standardLocations (
                QStandardPaths::TempLocation).at(0);
#endif

	cache_tmp_dest_ = s_.tmp_dest_;
	cache_tmp_src_ = s_.tmp_src_;
	cache_tmp_dest_.replace( "$TEMP", s_tmp );
	cache_tmp_src_.replace( "$TEMP", s_tmp );

	QTemporaryFile tf_s( cache_tmp_src_ );
	if ( tf_s.open() == false )
	{
		um.add( UserMsg::MSG_ERROR,	QObject::tr(
					"The input file %1 is unusable!" )
				.arg( tf_s.fileName() )
				);
		cache_tmp_src_.clear();
	}
	else
	{
		cache_tmp_src_ = tf_s.fileName();
		tf_s.close();
	}

	QTemporaryFile tf_d( cache_tmp_dest_ );
	if ( tf_d.open( ) == false )
	{
		um.add( UserMsg::MSG_ERROR,	QObject::tr(
					"The destination file %1 is unusable!" )
				.arg( tf_d.fileName() )
				);
		cache_tmp_dest_.clear();
	}
	else
	{
		cache_tmp_dest_ = tf_d.fileName();
		tf_d.close();
	}


	um.add( UserMsg::MSG_INFO,	QObject::tr(
				"Using input file: %1" )
			.arg( cache_tmp_src_ )
			);
	um.add( UserMsg::MSG_INFO,	QObject::tr(
				"Using output file: %1" )
			.arg( cache_tmp_dest_ )
			);


	/* program and arguments */
	cache_s_arg_ = s_.s_arg_;
	cache_s_arg_.replace( "$OUT", cache_tmp_dest_ );
	cache_s_arg_.replace( "$IN", cache_tmp_src_ );

	um.add( UserMsg::MSG_INFO,
			tr( "Generator program: " ) +
			s_.s_process_
			);
	um.add( UserMsg::MSG_INFO,
			tr( "Arguments: " ) +
			cache_s_arg_
			);

	um.show();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool				ImplProg::writeToTemp			(
		const QString & src_tx )
{
	UserMsg	um;

	QFile * f = new QFile( cache_tmp_src_ );
	if ( f->open( QIODevice::WriteOnly | QIODevice::Text ) == false )
	{
		delete f;
		um.add( UserMsg::MSG_INFO,
				tr( "Failed to open temporary file %1" )
				.arg( cache_tmp_src_ )
				);

		um.show();
		return false;
	}
	QTextStream tx( f );
	tx << src_tx;
	f->flush();
	f->close();
	delete f;

	return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::Stg::save				( void )
{
	QSettings stgs;
	stgs.setValue( "ImplProg/tmp_src", tmp_src_ );
	stgs.setValue( "ImplProg/tmp_dest", tmp_dest_ );
	stgs.setValue( "ImplProg/process_args", s_arg_ );
	stgs.setValue( "ImplProg/process", s_process_ );
	stgs.setValue( "ImplProg/log_start_end", b_log_start_end_ );
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::Stg::load				( void )
{
	QSettings stgs;
	tmp_src_ = stgs.value("ImplProg/tmp_src",
						  "$TEMP/dot_XXXXXXXX.dot" )
			.toString();
	tmp_dest_ = stgs.value("ImplProg/tmp_dest",
						   "$TEMP/dot_XXXXXXXX.png")
			.toString();
	s_arg_ = stgs.value("ImplProg/process_args",
						"-Tpng -o $OUT $IN" )
			.toString();

	s_process_ = stgs.value("ImplProg/process",
							"dot" )
			.toString();
	b_log_start_end_ = stgs.value("ImplProg/log_start_end",
								  false )
			.toBool();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ImplProg::Stg		ImplProg::settings				( void )
{
	if ( uniq_ == NULL )
	{
		Stg	s_ret;
		s_ret.load();
		return s_ret;
	}
	else
	{
		return uniq_->s_;
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::setSettings			( ImplProg::Stg & stg )
{
	if ( uniq_ == NULL )
	{
		stg.save();
	}
	else
	{
		uniq_->s_ = stg;
        stg.save();
		uniq_->applySettings();
	}

}
/* ========================================================================= */



/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */

