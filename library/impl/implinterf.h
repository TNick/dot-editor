/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			implinterf.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for ImplInterf class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __IMPLINTERF_INC__
#define __IMPLINTERF_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<QString>
#include	<QList>
#include	<QDebug>

class	QAction;
class	QPoint;
class	QGraphicsScene;

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/**
*	@brief	Interface for an implementation (back-end)
*/
class ImplInterf				{

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
	ImplInterf				()
	{}


	/**
	*	@brief	destructor;
	*/
	virtual					~ImplInterf				()
	{}


	/* ******************************************************************** */
	/** @name                   REQUIRED METHODS                            */
	/* ******************************************************************** */
	///@{


	/**
	*	@brief	Implementation is required to do its thing
	*/
	virtual bool			perform					() = 0;


	/**
	*	@brief	context menu for graphical display
	*/
	virtual void			ctxDisplay				(
			const QPoint &		loc,
			const QPoint &		glob_loc
			) = 0;


	/**
	*	@brief	the scene implemented by the implementation
	*/
	virtual QGraphicsScene*	scene					() = 0;


	/**
	*	@brief	action that toogles this mode on
	*/
	virtual QAction*		action					() = 0;


	///@}
	/* ******************************************************************** */


	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class ImplInterf	*/

/*  CLASS    =============================================================== */
//
//
//
//
#endif // __IMPLINTERF_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
