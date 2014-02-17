/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dfautomata.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for DFAutomata class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __DFAUTOMATA_INC__
#define __DFAUTOMATA_INC__
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


class	GVzEdge;
class	GVzNode;

/**
*	@brief	Deterministic Finite Automata
*
*	https://en.wikipedia.org/wiki/Deterministic_finite_automaton
*/
class DFAutomata	: public CmnAutomImpl 				{

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


	/**
	*	@brief	current node
	*/
	GVzNode *			crt_n_;


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
	DFAutomata			( PlugIns::Automata_PlugIn * plg_inst );


	/**
	*	@brief	destructor;
	*/
	~DFAutomata			( void );



	/* ******************************************************************* */
	/**
	*	@name Methods required to be implemented by CmnAutomImpl
	*/
	///@{


	/**
	*	@brief	report the kind of automata
	*/
	AutoTypes			kind				( void )
	{ return DFA; }


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

};	/*	class DFAutomata	*/

/*  CLASS    =============================================================== */
//
//
//
//
#endif // __DFAUTOMATA_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */

/*

digraph		graph_name	{
	a -> ba [label="0" tailport="w"]
	ba -> b [label="0" tailport="w"]
	ba -> a [label="1" tailport="e"]
	b -> ba [label="0" tailport="e"]
}

*/


