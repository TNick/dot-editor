/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dotviewer.h
  \date			Mar 2012
  \author		TNick

  \brief		Contains the definition for Settings class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __DOTVIEWER_INC__
#define __DOTVIEWER_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<QGraphicsView>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */


class DotViewer : public QGraphicsView
{
	Q_OBJECT

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
	*	@brief	zoom helper
	*/
	int			num_scheduled_scalings_;


	/*  DATA    ============================================================ */
	//
	//
	//
	//
	/*  FUNCTIONS    ------------------------------------------------------- */

private:


	/**
	*	@brief	common initialisation for all constructors
	*/
	void		init			( void );




public:


	/**
	*	@brief	constructor
	*/
	DotViewer(QWidget *parent = 0);


	/**
	*	@brief	constructor
	*/
	DotViewer(QGraphicsScene *scene, QWidget *parent = 0);


	/**
	*	@brief	destructor
	*/
	~DotViewer();


protected:

	/**
	*	@brief	zooming using this one
	*/
	void		wheelEvent		( QWheelEvent *event );


private slots:


	/**
	*	@brief	zooming utility
	*/
	void		scalingTime		( qreal x );


	/**
	*	@brief	zooming utility
	*/
	void		animFinished	( void );




	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};

/*  CLASS    =============================================================== */
//
//
//
//

#endif // __DOTVIEWER_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
