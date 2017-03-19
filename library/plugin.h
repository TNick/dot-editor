/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			plugin.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for PlugIn class


*//*

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __PLUGIN_INC__
#define __PLUGIN_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<QString>
#include	<QList>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/* following macro (PLUGIN_BUILD) should be defined with DEFINES += PLUGIN_BUILD
in the project for the plug-in */
#if defined(DE_PLUGIN_BUILD)
#  define PLGIN_EXPORT Q_DECL_EXPORT
#else
#  define PLGIN_EXPORT Q_DECL_IMPORT
#endif

class	UserMsg;

class	QToolBar;
class	QObject;

namespace	PlugIns	{

/**
*	@brief	class representing a (file based) plug-in loaded in memory
*/
class DE_EXPORT PlugIn				{

	//
	//
	//
	//
	/*  DEFINITIONS    ----------------------------------------------------- */

	friend class PlgInMan;

	/*  DEFINITIONS    ===================================================== */
	//
	//
	//
	//
	/*  DATA    ------------------------------------------------------------ */

private:


	/**
	*	@brief	associated file
	*/
	QString								file_;


protected:


	/**
	*	@brief	name of plug-in
	*/
	QString								name_;


	/*  DATA    ============================================================ */
	//
	//
	//
	//
	/*  FUNCTIONS    ------------------------------------------------------- */

public:


	/**
	*	@brief	constructor;
	*/
	PlugIn				 ();


	/**
	*	@brief	destructor;
	*/
	virtual				~PlugIn			 ();


	/**
	*	@brief	this function is called after the parent finished
	*	initialization procedures
	*
	*	Notably, the file name is set inside the instance when
	*	before the call is performed.
	*/
	virtual bool		initComplete	 (UserMsg & um);


	/**
	*	@brief	called just before Plug-in system is stopped
	*
	*	The plug-ins recieve this call from PlgInMan::systemClosing() and
	*	are free to delete themselves under this call. The plug-in pointer
	*	will be removed from the list and will never be used again.
	*
	*	Default implementation deletes the pointer, so plug-ins may handle
	*	the destructor and not concearn themselves with this function.
	*
	*	If, however, the instance is not allocated using new(), then this
	*	method should be implemented.
	*/
	virtual void		systemClosing	 (UserMsg & um);


	/**
	*	@brief	get name
	*/
	inline QString		name			() const
	{ return name_; }


	/**
	*	@brief	get the file that produced this plug-in
	*/
	inline QString		file			() const
	{ return file_; }



protected:


	/**
	*	@brief	set name
	*/
	inline void			setName		 (const QString & new_val)
	{ name_ = new_val; }


	/**
	*	@brief	set the file that produced this plug-in
	*/
	inline void			setFile		 (const QString & new_val)
	{ file_ = new_val; }


	/**
	*	@brief	create a toolbar in the name of this plug-in
	*/
	QToolBar *			createTb		(
			Qt::ToolBarArea			tb_a,
			const QString &			s_name
			);


	/**
	*	@brief	connect a slot to signal fired when the user changes the text
	*/
	void				connectTo_TxChanged (
			QObject *				dest_obj,
			const char *			dest_slt
			);



	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class PlugIn	*/

}	//	namespace	PlugIns


/**
*	@brief	the type of the function we're looking for
*/
typedef	PlugIns::PlugIn *		(*GetPlyGrInterf)			(
		char *					p_version,
		UserMsg &				um
		);


#ifdef		__cplusplus
extern "C" {
#endif


#define	STRINGIFY(a)	#a

/**
*	@brief	also define the name of the function to search (for QLibrary)
*/
#define	GET_INTERF_FUN			getPlugInInterface

/**
*	@brief	and the name as string
*/
#define	GET_INTERF_FUN_NAME		"getPlugInInterface"


#ifdef	PLUGIN_BUILD

/**
*	@brief	this function must be defined to get a pointer to plug-in interface
*
*/
PLGIN_EXPORT
PlugIns::PlugIn *		getPlugInInterface			(
		char *					p_version,	/* version string X.X.X */
		UserMsg &				um			/* error corelation structure */
		);

#endif


#ifdef		__cplusplus
}
#endif


/*  CLASS    =============================================================== */
//
//
//
//

#endif // __PLUGIN_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
