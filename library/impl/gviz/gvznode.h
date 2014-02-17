/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			gvznode.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for GVzNode class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __GVZNODE_INC__
#define __GVZNODE_INC__
#ifdef DE_WITH_GRAPHVIZ
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<QGraphicsItem>
#include	<graphviz/gvc.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

class	GVzEdge;

/**
*	@brief	represents a graphviz node in a QGraphicsScene
*/
class GVzNode	: public QGraphicsItem			{

	//
	//
	//
	//
	/*  DEFINITIONS    ----------------------------------------------------- */

public:


	enum	NdShape {
		SHP_UDEF = -1,

		SHP_MCIRCLE = 0,
		SHP_MDIAMOND,
		SHP_MSQUARE,
		SHP_BOX,
		SHP_BOX3D,
		SHP_CIRCLE,
		SHP_COMPONENT,
		SHP_DIAMOND,
		SHP_DOUBLECIRCLE,
		SHP_DOUBLEOCTAGON,
		SHP_EGG,
		SHP_ELLIPSE,
		SHP_FOLDER,
		SHP_HEXAGON,
		SHP_HOUSE,
		SHP_INVHOUSE,
		SHP_INVTRAPEZIUM,
		SHP_INVTRIANGLE,
		SHP_NONE,
		SHP_NOTE,
		SHP_OCTAGON,
		SHP_OVAL,
		SHP_PARALLELOGRAM,
		SHP_PENTAGON,
		SHP_PLAINTEXT,
		SHP_POINT,
		SHP_POLYGON,
		SHP_RECT,
		SHP_RECTANGLE,
		SHP_SEPTAGON,
		SHP_SQUARE,
		SHP_TAB,
		SHP_TRAPEZIUM,
		SHP_TRIANGLE,
		SHP_TRIPLEOCTAGON,

		SHP_MAX

	};


	/**
	*	@brief	the node in the dot graph
	*/
	enum		States		{
		STNONE = 0,
		ST_HIGHLITE

	};


	/*  DEFINITIONS    ===================================================== */
	//
	//
	//
	//
	/*  DATA    ------------------------------------------------------------ */

public:

	/**
	*	@brief	the node in the dot graph
	*/
	Agnode_t *		dot_n_;


private:

	/**
	*	@brief	the shape
	*/
	NdShape			shp_;


	/**
	*	@brief	parent node (the source)
	*/
	GVzNode *		fth_;


	/**
	*	@brief	list of edges leaving from this node
	*/
	QList<GVzEdge*>	edg_l_;


	/**
	*	@brief	a number of states are stored here
	*/
	States			st_lst_;


	/**
	*	@brief	node names
	*/
	static const char * shape_names[];


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
	GVzNode				(
			QGraphicsItem *		fth_g,
			Agnode_t *			node,
			GVzNode *			parent_n
			);


	/**
	*	@brief	destructor;
	*/
	~GVzNode			( void );


	/**
	*	@brief	re-examines the content and caches some values
	*/
	void				updateCachedData			( void );


	/**
	*	@brief	parent node; if this ia a top level node returns NULL
	*/
	inline GVzNode *	parentNode					( void )
	{ return fth_; }


	/**
	*	@brief	set parent node
	*/
	inline void			setParentNode				( GVzNode * new_val )
	{ fth_ = new_val; }


	/**
	*	@brief	append an edge to the list stored inside
	*/
	inline void			appendEdge					( GVzEdge * new_edg )
	{ edg_l_.append( new_edg ); }


	/**
	*	@brief	tell the shape to outter world
	*/
	inline NdShape		nodeShape					( void ) const
	{ return shp_; }


	/**
	*	@brief	tell the label to outter world
	*/
	QString				label						( void ) const;


	/**
	*	@brief	tell the number of edges that leave this node
	*/
	inline int			edgeCount					( void ) const
	{ return edg_l_.count(); }


	/**
	*	@brief	list of edges leaving from this node
	*/
	inline QList<GVzEdge*>	edges					( void ) const
	{ return edg_l_; }


	/**
	*	@brief	list of nodes that are tied to this one
	*/
	QList<GVzNode*>			nodes					( void ) const;


	/**
	*	@brief	change the visually apparence of the node
	*/
	void					setHighlite				( bool b_sts );


	/**
	*	@brief	tell if the node is highlited or not
	*/
	inline bool				isHighlited				( void ) const
	{ return ( ( st_lst_ & ST_HIGHLITE ) != 0 ); }


protected:

	QRectF			boundingRect			( void ) const;

	void			paint					(
			QPainter *							painter,
			const QStyleOptionGraphicsItem *	option,
			QWidget *							widget
			);


	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class GVzNode	*/

/*  CLASS    =============================================================== */
//
//
//
//
#endif // DE_WITH_GRAPHVIZ
#endif // __GVZNODE_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
