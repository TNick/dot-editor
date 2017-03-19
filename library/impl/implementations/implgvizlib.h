/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			implgvizlib.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for ImplGVizLib class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __IMPLGVIZLIB_INC__
#define __IMPLGVIZLIB_INC__
#ifdef	DE_WITH_GRAPHVIZ
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<impl/implinterf.h>

#include	<QGraphicsScene>
#include	<QMap>

#include	<graphviz/gvc.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

class	GVzNode;

/**
*	@brief	Implementation using Graphviz library
*
*	http://www.graphviz.org/
*/
class DE_EXPORT ImplGVizLib	: public ImplInterf			{

	//
	//
	//
	//
	/*  DEFINITIONS    ----------------------------------------------------- */

	/**
	*	@brief	settings for this implementation
	*/
	struct	Stg		{

		friend class ImplGVizLib;



	protected:

		/**
		*	@brief	dump to permanent storage
		*/
		void				save			 ();


		/**
		*	@brief	get from permanent storage
		*/
		void				load			 ();

	};

	/*  DEFINITIONS    ===================================================== */
	//
	//
	//
	//
	/*  DATA    ------------------------------------------------------------ */

private:

	/**
	*	@brief	cached settings
	*/
	Stg						s_;


	/**
	*	@brief	the scene for image display
	*/
	QGraphicsScene			img_scene;


	/**
	*	@brief	Graphviz context
	*/
	GVC_t *					context_;


	/**
	*	@brief	current graph
	*/
	Agraph_t *				graph_;


	/**
	*	@brief	top level nodes
	*/
	QList<GVzNode *>		tl_nodes_;




	/**
	*	@brief	the one and only instance
	*/
	static ImplGVizLib *	uniq_;


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
	ImplGVizLib			 ();


	/**
	*	@brief	destructor;
	*/
	~ImplGVizLib		 ();




public:

	/**
	*	@brief	Get a copy of the list containing top level nodes
	*/
	inline QList<GVzNode *>	headNodes			()
	{ return tl_nodes_; }


	/* ******************************************************************** */
	/** @name                  MANAGEMENT METHODS                           */
	/* ******************************************************************** */
	///@{


	/**
	*	@brief	Implementation is required to do its thing
	*/
	bool				perform					 ();


	/**
	*	@brief	context menu for graphical display
	*/
	void				ctxDisplay				(
			const QPoint &		loc,
			const QPoint &		glob_loc
			);


	/**
	*	@brief	the scene implemented by the implementation
	*/
	virtual QGraphicsScene*	scene				()
	{ return &img_scene; }


	/**
	*	@brief	associated action
	*/
	QAction*			action					 ();


	///@}
	/* ******************************************************************** */


private:


	/**
	*	@brief	"Signal" that the settings have changed
	*/
	void				applySettings			 ();




public:

	/**
	*	@brief	use this method to get the singleton (may create it)
	*/
	static ImplGVizLib *	unique			 (bool b_create = true)
	{ if (( uniq_ == NULL ) && b_create) new ImplGVizLib(); return uniq_; }


	/**
	*	@brief	tell the settings that govern us
	*/
	static Stg			settings				 ();


	/**
	*	@brief	change the settings that govern us
	*/
	static void			setSettings				 (Stg & stg);


	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class ImplGVizLib	*/

/*  CLASS    =============================================================== */
//
//
//
//
#endif // DE_WITH_GRAPHVIZ
#endif // __IMPLGVIZLIB_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
