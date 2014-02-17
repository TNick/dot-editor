/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			cmnautomimpl.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for CmnAutomImpl class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __CMNAUTOMIMPL_INC__
#define __CMNAUTOMIMPL_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

class	GVzEdge;
class	GVzNode;
class	UserMsg;

namespace	PlugIns		{
class		AutomataComObj;
class		Automata_PlugIn;
}

/**
*	@brief	The common ground for all implementations of automata
*/
class CmnAutomImpl				{

	//
	//
	//
	//
	/*  DEFINITIONS    ----------------------------------------------------- */

public:


	/**
	*	@brief	The AutoTypes enum describes the kinds of Automata we know of
	*/
	enum	AutoTypes		{
		DFA = 0,	/**< Deterministic Finite Automata */
		NFA,		/**< Non-deterministic Finite Automata */
		ENFA,		/**< Non-deterministic Finite Automata with epsilon */

		AT_MAX		/**< Validation */
	};


	/*  DEFINITIONS    ===================================================== */
	//
	//
	//
	//
	/*  DATA    ------------------------------------------------------------ */


protected:


	/**
	*	@brief	the plug-in instance
	*/
	PlugIns::Automata_PlugIn *		plg_;


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
	CmnAutomImpl			( PlugIns::Automata_PlugIn * plg_inst );


	/**
	*	@brief	destructor;
	*/
	virtual				~CmnAutomImpl		( void );


	/* ******************************************************************* */
	/** @name Methods required to be implemented by inheritants            */
	/* ******************************************************************* */
	///@{


	/**
	*	@brief	report the kind of automata
	*/
	virtual AutoTypes	kind				( void ) = 0;


	/**
	*	@brief	uses provided string and current state to update the status
	*/
	virtual void		goStep				( const QString & s_act ) = 0;


	/**
	*	@brief	reset the state, free cached objects,
	*/
	virtual void		reset				( void ) = 0;


	///@}
	/* ******************************************************************* */


	/**
	*	@brief	locates start-up node; returns NULL if not found
	*/
	GVzNode *			locateStartNode		(
			UserMsg &		um
			);



	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class CmnAutomImpl	*/

/*  CLASS    =============================================================== */
//
//
//
//
#endif // __CMNAUTOMIMPL_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
