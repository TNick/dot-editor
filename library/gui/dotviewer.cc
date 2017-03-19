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
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	"dotviewer.h"

#include	<QTimeLine>
#include	<QWheelEvent>
#include	<QMenu>
#include	<QDebug>


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void				DotViewer::init			()
{
	num_scheduled_scalings_ = 0;
	setDragMode (QGraphicsView::ScrollHandDrag);
/*
	setResizeAnchor (QGraphicsView::AnchorViewCenter);
	setRenderHints (QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setCacheMode (QGraphicsView::CacheBackground);
*/

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
DotViewer::DotViewer	 (QWidget *parent)
	:QGraphicsView( parent )
{
	init();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
DotViewer::DotViewer	 (QGraphicsScene *scene, QWidget *parent)
	:QGraphicsView( scene, parent )
{
	init();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
DotViewer::~DotViewer		()
{
	/* stub */
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				DotViewer::wheelEvent (QWheelEvent * event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;  // see QWheelEvent documentation
	num_scheduled_scalings_ += numSteps;
	// if user moved the wheel in another direction, we reset previously scheduled scalings
	if (num_scheduled_scalings_ * numSteps < 0 )
		num_scheduled_scalings_ = numSteps;

	QTimeLine *anim = new QTimeLine(350, this);
	anim->setUpdateInterval(20);

	connect( anim, SIGNAL(valueChanged(qreal) ),
			this, SLOT(scalingTime(qreal)) );
	connect( anim, SIGNAL(finished() ),
			this, SLOT(animFinished()) );
	anim->start();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				DotViewer::scalingTime	(qreal x)
{
	Q_UNUSED (x);
	qreal factor = 1.0 + qreal( num_scheduled_scalings_ ) / 1000.0;
	scale(factor, factor);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				DotViewer::animFinished	()
{
	if (num_scheduled_scalings_ > 0)
		num_scheduled_scalings_--;
	else
		num_scheduled_scalings_++;
	sender()->~QObject();
}
/* ========================================================================= */



/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
