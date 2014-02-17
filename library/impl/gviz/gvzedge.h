/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			gvzedge.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for GVzEdge class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __GVZEDGE_INC__
#define __GVZEDGE_INC__
#ifdef	DE_WITH_GRAPHVIZ
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<QGraphicsPathItem>
#include	<graphviz/gvc.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

class	GVzNode;

/**
*	@brief	represents a graphviz edge in a QGraphicsScene
*/
class GVzEdge	: public QGraphicsPathItem			{

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

public:


	/**
	*	@brief	the edge in the dot graph
	*/
	Agedge_t *		dot_e_;


private:

	/**
	*	@brief	source node (from where this leaves)
	*/
	GVzNode *		src_;


	/**
	*	@brief	destination node (where this ends)
	*/
	GVzNode *		dst_;


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
	GVzEdge					(
			QGraphicsPathItem *		parent,
			Agedge_t *				edge,
			GVzNode *				src,
			GVzNode *				dst
			);


	/**
	*	@brief	destructor;
	*/
	~GVzEdge				( void );


	/**
	*	@brief	re-examines the content and caches some values
	*/
	void					updateCachedData			( void );


	/**
	*	@brief	source node (from where this leaves)
	*/
	inline GVzNode *		source		( void )
	{ return src_; }


	/**
	*	@brief	destination node (where this ends)
	*/
	inline GVzNode *		destination	( void )
	{ return dst_; }


	/**
	*	@brief	set destination node (where this ends)
	*/
	inline void				setDestination	( GVzNode * new_v )
	{ dst_ = new_v; }


	/**
	*	@brief	tell the label to outter world
	*/
	QString					label						( void ) const;


	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class GVzEdge	*/

/*  CLASS    =============================================================== */
//
//
//
//
#endif // DE_WITH_GRAPHVIZ
#endif // __GVZEDGE_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
