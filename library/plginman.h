/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plginman.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for PlgInMan class


*//*

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __PLGINMAN_INC__
#define __PLGINMAN_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<QList>
#include	<QStringList>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

class	UserMsg;
class	QLibrary;

namespace	PlugIns	{

class	PlugIn;

/**
*	@brief	class that manages the plug-ins
*/
class DE_EXPORT PlgInMan				{

	//
	//
	//
	//
	/*  DEFINITIONS    ----------------------------------------------------- */

public:


	/**
	*	@brief	information about a plug-in
	*/
	struct		PlgInInfo	{
		QString		s_full_path;
		QString		s_name;
		PlugIn *	p_interf;
		bool		b_start_up;
		bool		b_loaded;
	};

	/*  DEFINITIONS    ===================================================== */
	//
	//
	//
	//
	/*  DATA    ------------------------------------------------------------ */

private:


	/**
	*	@brief	the list of loaded plug-ins
	*/
	QList<PlugIn* >		plgin_list_;

	/**
	*	@brief	the list of loaded libraries
	*/
	QList<QLibrary* >	lib_list_;


	/**
	*	@brief	singleton
	*/
	static PlgInMan *		uniq_;

	/*  DATA    ============================================================ */
	//
	//
	//
	//
	/*  FUNCTIONS    ------------------------------------------------------- */

private:

	/**
	*	@brief	constructor;
	*/
	PlgInMan			( void )			{}


	/**
	*	@brief	destructor;
	*/
	~PlgInMan			( void )			{}


public:


	/**
	*	@brief	initialisation method for plug-in system
	*
	*	Will see if there are some plug-ins that should be loaded at at
	*	start-up and, if so, will load them
	*/
	static void			init			( void );


	/**
	*	@brief	termination method for plug-in system
	*
	*	Will send PlugIn::systemClosing() to each plug-in in list. The plug-ins
	*	are free to delete themselves under this call. The plug-in pointer
	*	will be removed from the list and will never be used again.
	*/
	static void			end						( void );


	/**
	*	@brief	append a plug-in instance to the manager
	*/
	static void			append					( PlugIns::PlugIn * plg_in )
	{ uniq_->plgin_list_.append( plg_in ); }


	/**
	*	@brief	remove a plug-in instance from the manager
	*/
	static void			remove					( PlugIns::PlugIn * plg_in )
	{ uniq_->plgin_list_.removeOne( plg_in ); }


	/**
	*	@brief	get the plug-in at index i
	*/
	static PlugIns::PlugIn*		plugin			( int i );


	/**
	*	@brief	get the number of plug-ins that we know about
	*/
	static int			count					( void )
	{ return uniq_->plgin_list_.count(); }


	/**
	*	@brief	load a single plug-in from specified file
	*/
	static OUTCOME				fromFile		(
			const QString &			s_file,	/**< path */
			PlugIn **				out_p,	/**< resulting plug-in */
			UserMsg &				um		/**< errors reported here */
			);


	/**
	*	@brief	unload a plug-in
	*
	*	If the provenience of the plug-in is not the same as the one that
	*	was specified the plug-in is not unloaded.
	*/
	static	void				unload			(
			PlugIn *				the_p	/**< the plug-in */
			);

	/**
	*	@brief	adds the plug-in to the start-up suite
	*/
	static	OUTCOME makeStartUp(
			PlugIn *				the_p,	/**< the plug-in */
			UserMsg &				um		/**< errors reported here */
			);


	/**
	*	@brief	adds the plug-in to the start-up suite
	*/
	static	OUTCOME				makeStartUp		(
			QString &				the_p,	/**< the file for plug-in */
			UserMsg &				um		/**< errors reported here */
			);


	/**
	*	@brief	remove a plug-in from the start-up suite
	*
	*/
	static	OUTCOME				remFromStartUp	(
			QString &				the_p,	/**< the file for plug-in */
			UserMsg &				um		/**< errors reported here */
			);


	/**
	*	@brief	return a list of plug-ins along wth their load state
	*
	*	The method lists plug-ins from the search path. It marks the ones that
	*	are loaded and the ones that are part of the start-up suite.
	*
	*	@see	syncListPlugIns
	*/
	static	QList<PlgInInfo>	listPlugIns	( QString & s_plg_path );


	/**
	*	@brief	syncronises the current status with the one in list
	*
	*	The function may load or unload a plug-in. Only the files marked
	*	in this structure will be loaded at start-up from now on.
	*
	*	@see	listPlugIns
	*/
	static	OUTCOME				syncListPlugIns	(
			QList<PlgInInfo> &		lst,	/**< adjusted list */
			UserMsg &				um		/**< errors reported here */
			);


private:

	/**
	*	@brief	get the start-up suite list
	*/
	static QStringList			startUpSuite		( void );


	/**
	*	@brief	set the start-up suite list
	*/
	static void					setStartUpSuite		(
			QStringList &			new_l
			);


	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class PlgInMan	*/

/*  CLASS    =============================================================== */
//
//
//
//

}	//	namespace	PlugIns

#endif // __PLGINMAN_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
