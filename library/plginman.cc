/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plginman.cc
  \date			May 2012
  \author		TNick

  \brief		Contains the implementation for PlgInMan class


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

#include	<QLibrary>
#include	<QSettings>
#include	<QDir>
#include	<QLibrary>

#include	<plugin.h>
#include	<usermsg.h>

#include	"plginman.h"

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

PlgInMan *		PlgInMan::uniq_ = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
QStringList		PlgInMan::startUpSuite		( void )
{
	QSettings	stg;
	QStringList sl = stg.value( "stg/startUpSuite" ).toStringList();

	return sl;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			PlgInMan::setStartUpSuite		( QStringList & new_l )
{
	QSettings	stg;
	stg.setValue( "stg/startUpSuite", new_l );
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			PlgInMan::end		( void )
{

	UserMsg		um;

	int i = uniq_->plgin_list_.count() - 1;
	while ( i >= 0)
	{
		uniq_->plgin_list_.at( i )->systemClosing( um );
		uniq_->lib_list_.at( i )->unload();
		delete uniq_->lib_list_.at( i );
		i--;
	}

	uniq_->plgin_list_.clear();
	uniq_->lib_list_.clear();

	if ( um.count() > 0 )
		um.show();


	delete uniq_;
	uniq_ = NULL;

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void		PlgInMan::init		( void )
{
	UserMsg		um;
	QStringList sl = startUpSuite();

	uniq_ = new PlgInMan();

	bool	b_ok = true;
	QString	s_item;
	foreach(s_item, sl)	{
		PlugIn *		out_p;
		b_ok &= ( OUT_SUCCESS( fromFile( s_item, &out_p, um) ) );
	}

	if ( ! b_ok )
	{
		um.show();
	}

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
PlugIn*			PlgInMan::plugin			( int i )
{
	if ( i < uniq_->plgin_list_.count( ) )
	{
		return uniq_->plgin_list_.at( i );
	}
	return NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
OUTCOME				PlgInMan::fromFile (
		const QString & s_file,
		PlugIn **out_p, UserMsg & um )
{
	QLibrary * 		plg_lib = new QLibrary();
	GetPlyGrInterf	the_func;

	*out_p = NULL;

	/* attempt to load the library */
	plg_lib->setFileName( s_file );
	if ( ! plg_lib->isLoaded() )
	{
		if ( ! plg_lib->load() )
		{
			um.add( UserMsg::MSG_ERROR,	QObject::tr(
						"Could not load plugin %1.\n%2" )
					.arg( s_file )
					.arg( plg_lib->errorString() )
					);
			delete plg_lib;
			return OUTCOME_ERROR;
		}
		the_func = (GetPlyGrInterf) plg_lib->resolve( GET_INTERF_FUN_NAME );
		if ( the_func )
		{
            static char ver[] = { DOTEDITOR_VERSION_S };
			PlugIn * ptr_plg = the_func( &ver[ 0 ], um );
			if ( ptr_plg == NULL )
			{
				um.add( UserMsg::MSG_ERROR,	QObject::tr(
							"The plug-in %1 did not provide an "
							"interface uppon request." )
						.arg( plg_lib->fileName() )
						);
				plg_lib->unload();
				delete plg_lib;
				return OUTCOME_ERROR;
			}
			else
			{
				ptr_plg->setFile( plg_lib->fileName() );
				if ( ! ptr_plg->initComplete( um ) )
				{
					delete ptr_plg;
				}
				*out_p = ptr_plg;
				uniq_->lib_list_.append( plg_lib );
				return OUTCOME_OK;
			}
		}
		else
		{
			um.add( UserMsg::MSG_ERROR,	QObject::tr(
						"The library %1 is not a valid plug-in.\n%2" )
					.arg( plg_lib->fileName() )
					.arg( plg_lib->errorString() )
					);
			plg_lib->unload();
			delete plg_lib;
			return OUTCOME_ERROR;
		}
	}

	return OUTCOME_OK;

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				PlgInMan::unload	( PlugIn * the_p )
{
	if ( the_p == NULL)
		return;

	int i = uniq_->plgin_list_.indexOf( the_p );
	if ( i != -1 )
	{
		Q_ASSERT( i < uniq_->lib_list_.count() );
		uniq_->plgin_list_.removeAt( i );
		uniq_->lib_list_.removeAt( i );
		delete the_p;
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
OUTCOME				PlgInMan::makeStartUp	(
		PlugIn * the_p, UserMsg & um  )
{
	if ( uniq_->plgin_list_.indexOf( the_p ) == -1 )
	{
		uniq_->plgin_list_.append( the_p );
	}
	QString sf = the_p->file();
	return makeStartUp( sf, um );
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
OUTCOME				PlgInMan::remFromStartUp(
		QString & the_p, UserMsg & um )
{

	if ( the_p.isEmpty() )
	{
		um.add( UserMsg::MSG_ERROR, QObject::tr(
					"Attempt to remove start-up plug-in but"
					"no plug-in file provided.")
				);
		return OUTCOME_ERROR;
	}

	QStringList sl = startUpSuite();
	for ( int i = 0; i < sl.count(); i++ )
	{
		if ( 0 == sl.at( i ).compare( the_p, Qt::CaseInsensitive ) )
		{
			sl.removeAt( i );
			setStartUpSuite( sl );
			return OUTCOME_OK;
		}
	}
	um.add( UserMsg::MSG_WARNING, QObject::tr(
				"Attempt to remove start-up plug-in - "
				"a file (%1) that is not part of start-up suite")
			.arg( the_p )
			);
	return OUTCOME_ERROR;

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QList<PlgInMan::PlgInInfo> PlgInMan::listPlugIns		(
		QString & s_plg_path )
{
	QFileInfo			fi;
	PlugIn*				plgin;
	PlgInInfo			crt_info;
	QSettings			stg;
	QList<PlgInInfo>	plist;
	QStringList			st_s = startUpSuite();
	QString s_def = QCoreApplication::applicationDirPath() + "/plugins";
	QDir	d( stg.value( "plgin/dir", s_def ).toString() );
	if ( d.exists() == false )
	{
		d.setPath( s_def );
		if ( d.exists() == false )
		{
			s_plg_path = s_def;
			return plist;
		}
	}
	s_plg_path = d.absolutePath();
	QFileInfoList		fi_list = d.entryInfoList();

	foreach( fi, fi_list )		{
		if ( fi.isSymLink() )
			continue;
		crt_info.s_full_path = fi.absoluteFilePath();
		if ( QLibrary::isLibrary( crt_info.s_full_path ) )
		{
			crt_info.b_loaded = false;
			crt_info.p_interf = NULL;
			foreach( plgin, uniq_->plgin_list_ )		{
				if ( plgin->file_ == crt_info.s_full_path )
				{ /* it is loaded */
					crt_info.b_loaded = true;
					crt_info.s_name = plgin->name();
					crt_info.p_interf = plgin;
					break;
				}
			}
			if ( crt_info.b_loaded == false )
			{
				crt_info.s_name = fi.baseName();
			}
			crt_info.b_start_up = st_s.contains( crt_info.s_full_path );
			plist.append( crt_info );
		}
	}

	return plist;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
OUTCOME					PlgInMan::syncListPlugIns		(
		QList<PlgInInfo> & lst, UserMsg & um )
{
	QSettings			stg;
	PlgInInfo			crt_i;
	PlugIn*				plgin;
	bool				b_ok = true;
	OUTCOME				b_var = OUTCOME_OK;
	QStringList			st_s;

	foreach( crt_i, lst )		{
		if ( ( crt_i.b_loaded ) && ( crt_i.p_interf == NULL ) )
		{ /* load */
			b_ok &= OUT_SUCCESS( PlgInMan::fromFile(
						crt_i.s_full_path,
						&crt_i.p_interf,
						um
						) );
		}
		else if ( ( crt_i.b_loaded == false ) && ( crt_i.p_interf != NULL ) )
		{ /* unload */
			foreach( plgin, uniq_->plgin_list_ )		{
				if ( plgin->file_ == crt_i.s_full_path )
				{
					Q_ASSERT( plgin == crt_i.p_interf );
					unload( crt_i.p_interf );
					crt_i.b_loaded = false;
					crt_i.p_interf = NULL;
					break;
				}
			}
			/* if we did not found thi one...*/
			if ( crt_i.b_loaded )
			{
				um.add( UserMsg::MSG_WARNING,	QObject::tr(
							"Plugin %1 is not loaded!" )
						.arg( crt_i.s_full_path )
						);
				b_var = OUTCOME_WARNING;
			}
		}

		if ( crt_i.b_start_up )
		{
			st_s.append( crt_i.s_full_path );
		}
	}
	setStartUpSuite( st_s );
	if ( b_ok )
		return b_var;
	else
		return OUTCOME_ERROR;

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
OUTCOME					PlgInMan::makeStartUp			(
		QString & the_p, UserMsg & um )
{

	if ( the_p.isEmpty() )
	{
		um.add( UserMsg::MSG_ERROR, QObject::tr(
					"Attempt to make start-up plug-in but"
					"no plug-in file provided.")
				);
		return OUTCOME_ERROR;
	}

	QFile	s_file( the_p );
	if ( ! s_file.exists() )
	{
		um.add( UserMsg::MSG_ERROR, QObject::tr(
					"Attempt to make start-up plug-in with "
					"a non-existing file (%1)")
				.arg( the_p )
				);
		return OUTCOME_ERROR;
	}

	QStringList sl = startUpSuite();
	for ( int i = 0; i < sl.count(); i++ )
	{
		QFile	s_file_c( sl.at( i ) );
		if ( s_file.fileName() == s_file_c.fileName() )
		{
			um.add( UserMsg::MSG_WARNING, QObject::tr(
						"Attempt to make start-up plug-in with "
						"a file (%1) that is already part of start-up suite")
					.arg( the_p )
					);
			return OUTCOME_ERROR;
		}
	}

	sl.append( the_p );
	setStartUpSuite( sl );

	return OUTCOME_OK;
}
/* ========================================================================= */

/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
