/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			implmanager.h
  \date			Aug 2012
  \author		TNick

  \brief		Contains the definition for ImplManager class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __IMPLMANAGER_INC__
#define __IMPLMANAGER_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<QList>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

class	ImplInterf;

/**
*	@brief	Manages the interfaces
*/
class ImplManager				{

	//
	//
	//
	//
	/*  DEFINITIONS    ----------------------------------------------------- */

public:

	/**
	*	@brief	build-in implementations; these can be constructed using constants
	*/
	enum		BIn		{
		IMPL_PROG,	/**< uses external programs */
		IMPL_GVIZ,	/**< uses the graphviz library*/

		IMPL_MAX
	};

	/**
	*	@brief	callback that creates an implementation
	*/
	typedef ImplInterf * (*kbCreateImpl)	( void );


	/*  DEFINITIONS    ===================================================== */
	//
	//
	//
	//
	/*  DATA    ------------------------------------------------------------ */

private:

	/**
	*	@brief	list of implementations
	*/
	QList<kbCreateImpl>			impl_lst_;


	/**
	*	@brief	singleton
	*/
	static ImplManager *		uniq_;


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
	ImplManager			( void );


	/**
	*	@brief	destructor;
	*/
	~ImplManager			( void );



public:


	/**
	*	@brief	initialisation method
	*
	*	Will prepare the list of available callbacks with predefined ones.
	*	It does not set a current implementation, allowing plug-ins to
	*	create their own implementations to be added to the list.
	*
	*	The gui will call setImpl() with the index that was saved in settings.
	*/
	static bool					init					( void );


	/**
	*	@brief	termination method
	*/
	static void					end						( void );


	/**
	*	@brief	termination method
	*/
	static inline ImplManager *	unique					( void )
	{ return uniq_; }


	/**
	*	@brief	add an callback capable of creating an implementation
	*/
	static inline void		addImplem				( kbCreateImpl kb_loc )
	{
		if ( kb_loc == NULL ) return;
		uniq_->impl_lst_.append( kb_loc );
	}


	/**
	*	@brief	add an callback capable of creating an implementation
	*/
	static inline bool		remImplem				( kbCreateImpl kb_loc )
	{
		int idx = uniq_->impl_lst_.indexOf( kb_loc );
		if ( idx < IMPL_MAX ) return false;
		uniq_->impl_lst_.removeAt( idx );
		return true;
	}


	/**
	*	@brief	callback capable of creating an implementation at index \b idx
	*/
	static inline kbCreateImpl 	getImplCallback			( int idx )
	{
		if ( ( idx < 0 ) || ( idx >= uniq_->impl_lst_.count() ) )
			return NULL;
		return uniq_->impl_lst_.at( idx );
	}


	/**
	*	@brief	tell if the ImplGVizLib back-end is present in this binary
	*/
	static bool			hasGViz					( void );


	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class ImplManager	*/

/*  CLASS    =============================================================== */
//
//
//
//
#endif // __IMPLMANAGER_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
