/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			enfautomata.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for ENFAutomata class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __ENFAUTOMATA_INC__
#define __ENFAUTOMATA_INC__
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
*	@brief	Non-deterministic Finite Automata with epsilon
*
*	https://en.wikipedia.org/wiki/Nondeterministic_finite_automaton_with_%CE%B5-moves
*/
class ENFAutomata	: public CmnAutomImpl				{

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
	ENFAutomata			( PlugIns::Automata_PlugIn * plg_inst );


	/**
	*	@brief	destructor;
	*/
	~ENFAutomata		( void );



	/* ******************************************************************* */
	/**
	*	@name Methods required to be implemented by CmnAutomImpl
	*/
	///@{


	/**
	*	@brief	report the kind of automata
	*/
	AutoTypes			kind				( void )
	{ return ENFA; }


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

};	/*	class ENFAutomata	*/

/*  CLASS    =============================================================== */
//
//
//
//
#endif // __ENFAUTOMATA_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
