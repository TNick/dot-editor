/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			nfautomata.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for NFAutomata class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __NFAUTOMATA_INC__
#define __NFAUTOMATA_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	"cmnautomimpl.h"

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/**
*	@brief	Non-deterministic Finite Automata without epsilon
*
*	https://en.wikipedia.org/wiki/Nondeterministic_finite_automata
*/
class NFAutomata	: public CmnAutomImpl				{

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
	NFAutomata			( PlugIns::Automata_PlugIn * plg_inst );


	/**
	*	@brief	destructor;
	*/
	~NFAutomata			( void );



	/* ******************************************************************* */
	/**
	*	@name Methods required to be implemented by CmnAutomImpl
	*/
	///@{


	/**
	*	@brief	report the kind of automata
	*/
	AutoTypes			kind				( void )
	{ return NFA; }


	/**
	*	@brief	uses provided string and current state to update the status
	*/
	void				goStep				( const QString & s_act );


	/**
	*	@brief	reset the state, free cached objects,
	*/
	void				reset				( void );


	///@}
	/* ******************************************************************* */


	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class NFAutomata	*/

/*  CLASS    =============================================================== */
//
//
//
//
#endif // __NFAUTOMATA_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
