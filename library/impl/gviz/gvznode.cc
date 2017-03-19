/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			gvznode.cc
  \date			May 2012
  \author		TNick

  \brief		Contains the implementation of GVzNode class


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
#ifdef	DE_WITH_GRAPHVIZ
#include	<dot-editor.h>

#include	<QDebug>
#include	<QPainter>
#include	<QBrush>
#include	<QPen>
#include	<QFont>
#include	<QStyleOptionGraphicsItem>

#include	<impl/gviz/gvzedge.h>

#include	"gvznode.h"


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */


/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

/* related to NdShape */
const char *			GVzNode::shape_names[] = {
	"Mcircle",
	"Mdiamond",
	"Msquare",
	"box",
	"box3d",
	"circle",
	"component",
	"diamond",
	"doublecircle",
	"doubleoctagon",
	"egg",
	"ellipse",
	"folder",
	"hexagon",
	"house",
	"invhouse",
	"invtrapezium",
	"invtriangle",
	"none",
	"note",
	"octagon",
	"oval",
	"parallelogram",
	"pentagon",
	"plaintext",
	"point",
	"polygon",
	"rect",
	"rectangle",
	"septagon",
	"square",
	"tab",
	"trapezium",
	"triangle",
	"tripleoctagon",
	0
};


/*  DATA    ================================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
GVzNode::GVzNode	(
		QGraphicsItem * fth_g, Agnode_t * node, GVzNode * parent_n )
	: QGraphicsItem( fth_g )
{
	setFlags( ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
	dot_n_ = node;
	fth_ = parent_n;
	st_lst_ = STNONE;

	updateCachedData();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
GVzNode::~GVzNode	()
{
	edg_l_.clear();
}
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
QString					GVzNode::label						() const
{
	if (dot_n_ == NULL)
		return QString();

	textlabel_t * tl = ND_label (dot_n_);
	if (tl == NULL)
		return QString();

	return tl->text;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QRectF					GVzNode::boundingRect		() const
{
	qreal penWidth = 1;
	qreal	w = ND_width( dot_n_ ) * 72;
	qreal	h = ND_height( dot_n_ ) * 72;

	return QRectF(
				- ( w / 2 + penWidth / 2 ),
				- ( h / 2 + penWidth / 2 ),
				w + penWidth,
				h + penWidth
				);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void					octogonInRect	 (QRectF & r, QPointF * pts)
{
	/* 9 POINTS EXPECTED */

	qreal x_3 = r.width() / 3;
	qreal y_3 = r.height() / 3;
	qreal x_2_3 = x_3 * 2;
	qreal y_2_3 = y_3 * 2;

	pts[0].setX (r.left() );			pts[0].setY( r.top() + y_3);
	pts[1].setX (r.left() + x_3 );		pts[1].setY( r.top());
	pts[2].setX (r.left() + x_2_3 );	pts[2].setY( r.top());
	pts[3].setX (r.right() );			pts[3].setY( r.top() + y_3);
	pts[4].setX (r.right() );			pts[4].setY( r.top() + y_2_3);
	pts[5].setX (r.left() + x_2_3 );	pts[5].setY( r.bottom());
	pts[6].setX (r.left() + x_3 );		pts[6].setY( r.bottom());
	pts[7].setX (r.left() );			pts[7].setY( r.top() + y_2_3);
	pts[8] = pts[0];

}
/* ========================================================================= */

#define	q_agfindattr(a,b)	agfindattr( a, const_cast<char *>( b ) )

/* ------------------------------------------------------------------------- */
void					GVzNode::paint(
		QPainter * painter, const QStyleOptionGraphicsItem * option,
		QWidget * widget )
{
	Q_UNUSED (option);
	Q_UNUSED (widget);
	QPointF	pts[16];
	QRectF	bb2;
	qreal	a;

	if (dot_n_ == NULL)
		return;

	Agsym_t * atr_clr = q_agfindattr (dot_n_, "color");
	Agsym_t * atr_fillclr = q_agfindattr (dot_n_, "fillcolor");
	Agsym_t * atr_bgcolor = q_agfindattr (dot_n_, "bgcolor");
	Agsym_t * atr_style = q_agfindattr (dot_n_, "style");
	//	Agsym_t * atr_fontcolor = q_agfindattr (dot_n_, "fontcolor");
	//	Agsym_t * atr_fontname = q_agfindattr (dot_n_, "fontname");
	//	Agsym_t * atr_fontsize = q_agfindattr (dot_n_, "fontsize");
	//	Agsym_t * atr_label = q_agfindattr (dot_n_, "label");

	char * vl;
	if (atr_clr != NULL){
		vl = agxget (dot_n_, atr_clr->index);
		painter->setPen (QColor( vl ).toRgb());
	}
	if (atr_fillclr != NULL){
		QBrush	brs;
		vl = agxget (dot_n_, atr_fillclr->index);
		brs.setColor (QColor( vl ));
		brs.setStyle (Qt:: SolidPattern);
		painter->setBrush (brs);
	}
	if (atr_bgcolor != NULL){
		/* ? where to use this? */
		vl = agxget (dot_n_, atr_bgcolor->index);
	}
	if (atr_style != NULL){
		/* ? where to use this? */
		vl = agxget (dot_n_, atr_style->index);
	}


	QRectF	bb = boundingRect();
	if (isHighlited())
	{
		painter->setPen (QColor( Qt::red ));
		painter->setBrush (QBrush( QColor( 0x00, 0xf5, 0xf5, 0x50 ) ));
	}

	/* draw based on provided shape */
	switch ( shp_ )	{
	case	SHP_RECT:
	case	SHP_RECTANGLE:
	case	SHP_SQUARE:
	case	SHP_BOX:
		painter->drawRect (bb);
		break;
	case	SHP_NONE:
		break;

	case	SHP_MCIRCLE:		{
		a = bb.height() / 8;
		bb2 = bb.adjusted( a, a, -a, -a);
		painter->drawEllipse (bb);
		painter->drawRect (bb2);
		break;}

	case	SHP_MDIAMOND:
	case	SHP_MSQUARE:		{
		painter->drawRect (bb);

		pts[0].setX (bb.left() );		pts[0].setY( bb.top()+bb.height()/2);
		pts[1].setX (bb.left()+bb.width()/2 );	pts[1].setY( bb.top());
		pts[2].setX (bb.right() );		pts[2].setY( pts[0].y());
		pts[3].setX (pts[1].x() );		pts[3].setY( bb.bottom());
		painter->drawConvexPolygon (&pts[0], 4);
		break;}

	case	SHP_CIRCLE:
	case	SHP_ELLIPSE:
	case	SHP_OVAL:
	case	SHP_EGG:
		painter->drawEllipse (bb);
		break;

	case	SHP_DOUBLECIRCLE:	{
		a = 2;//bb.width() / 10;
		bb2 = bb.adjusted( a, a, -a, -a);
		painter->drawEllipse (bb);
		painter->drawEllipse (bb2);
		break;}

	case	SHP_BOX3D:			{
		a = bb.width() / 8;

		bb2 = bb.adjusted (0, a, -a, 0);
		painter->drawRect (bb2);

		pts[0].setX (bb2.left() );		pts[0].setY( bb2.top());
		pts[1].setX (bb2.left()+a );	pts[1].setY( bb.top());
		pts[2].setX (bb.right() );		pts[2].setY( bb.top());
		pts[3].setX (bb.right() );		pts[3].setY( bb2.bottom()-a);
		pts[4].setX (bb2.right() );		pts[4].setY( bb2.bottom());
		painter->drawPolyline (&pts[0], 5);

		painter->drawLine (pts[2], bb2.topRight());
		break;}

	case	SHP_COMPONENT:		{
		a = bb.width() / 12;

		bb2 = bb.adjusted (a, 0, 0, 0);
		painter->drawRect (bb2);

		bb2 = QRectF (bb.left(), bb.top()+a, a*2, a);
		painter->drawRect (bb2);

		bb2.translate (0, bb.height() - a*3);
		painter->drawRect (bb2);


		break;}

	case	SHP_DIAMOND:		{
		pts[0].setX (bb.left() );		pts[0].setY( bb.top()+bb.height()/2);
		pts[1].setX (bb.left()+bb.width()/2 );	pts[1].setY( bb.top());
		pts[2].setX (bb.right() );		pts[2].setY( pts[0].y());
		pts[3].setX (pts[1].x() );		pts[3].setY( bb.bottom());
		painter->drawConvexPolygon (&pts[0], 4);
		break;}

	case	SHP_TRIPLEOCTAGON:	{
		a = bb.height() / 16;
		bb2 = bb.adjusted (a, a, -a, -a);
		octogonInRect (bb, &pts[0]);
		painter->drawPolyline (&pts[0], 9);
		octogonInRect (bb2, &pts[0]);
		painter->drawPolyline (&pts[0], 9);
		bb2 = bb2.adjusted (a, a, -a, -a);
		octogonInRect (bb2, &pts[0]);
		painter->drawConvexPolygon (&pts[0], 8);
		break;}


	case	SHP_DOUBLEOCTAGON:	{
		a = bb.height() / 16;
		bb2 = bb.adjusted (a, a, -a, -a);
		octogonInRect (bb, &pts[0]);
		painter->drawPolyline (&pts[0], 9);
		octogonInRect (bb2, &pts[0]);
		painter->drawConvexPolygon (&pts[0], 8);
		break;}

	case	SHP_OCTAGON:	{
		octogonInRect (bb, &pts[0]);
		painter->drawConvexPolygon (&pts[0], 8);
		break;}

	case	SHP_HEXAGON:	{
		a = bb.height() / 2;
		pts[0].setX (bb.left() );		pts[0].setY( bb.top()+a);
		pts[1].setX (bb.left()+a );		pts[1].setY( bb.top());
		pts[2].setX (bb.right()-a );	pts[2].setY( bb.top());
		pts[3].setX (bb.right() );		pts[3].setY( bb.top()+a);
		pts[4].setX (bb.right()-a );	pts[4].setY( bb.bottom());
		pts[5].setX (bb.left()+a );		pts[5].setY( bb.bottom());

		painter->drawConvexPolygon (&pts[0], 6);
		break;}

	case	SHP_HOUSE:		{
		a = bb.height() / 3;
		pts[0].setX (bb.left() );		pts[0].setY( bb.top()+a);
		pts[1].setX (bb.left()+bb.width()/2 );	pts[1].setY( bb.top());
		pts[2].setX (bb.right() );		pts[2].setY( bb.top()+a);
		pts[3].setX (bb.right() );		pts[3].setY( bb.bottom());
		pts[4].setX (bb.left() );		pts[4].setY( bb.bottom());

		painter->drawConvexPolygon (&pts[0], 5);
		break;}

	case	SHP_INVHOUSE:		{
		a = bb.height() / 3;
		pts[0].setX (bb.left() );		pts[0].setY( bb.bottom()-a);
		pts[1].setX (bb.left()+bb.width()/2 );	pts[1].setY( bb.bottom());
		pts[2].setX (bb.right() );		pts[2].setY( bb.bottom()-a);
		pts[3].setX (bb.right() );		pts[3].setY( bb.top());
		pts[4].setX (bb.left() );		pts[4].setY( bb.top());

		painter->drawConvexPolygon (&pts[0], 5);
		break;}

	case	SHP_TRIANGLE:		{
		pts[0].setX (bb.left() );				pts[0].setY( bb.bottom());
		pts[1].setX (bb.left()+bb.width()/2 );	pts[1].setY( bb.top());
		pts[2].setX (bb.right() );				pts[2].setY( bb.bottom());

		painter->drawConvexPolygon (&pts[0], 3);
		break;}

	case	SHP_INVTRIANGLE:		{
		pts[0].setX (bb.left() );				pts[0].setY( bb.top());
		pts[1].setX (bb.left()+bb.width()/2 );	pts[1].setY( bb.bottom());
		pts[2].setX (bb.right() );				pts[2].setY( bb.top());

		painter->drawConvexPolygon (&pts[0], 3);
		break;}

	case	SHP_TRAPEZIUM:			{
		a = bb.width() / 8;
		pts[0].setX (bb.left() );				pts[0].setY( bb.bottom());
		pts[1].setX (bb.left()+a );				pts[1].setY( bb.top());
		pts[2].setX (bb.right()-a );			pts[2].setY( bb.top());
		pts[3].setX (bb.right() );				pts[3].setY( bb.bottom());
		pts[4] = pts[0];

		painter->drawConvexPolygon (&pts[0], 4);
		break;}

	case	SHP_INVTRAPEZIUM:		{
		a = bb.width() / 8;
		pts[0].setX (bb.left() );				pts[0].setY( bb.top());
		pts[1].setX (bb.left()+a );				pts[1].setY( bb.bottom());
		pts[2].setX (bb.right()-a );			pts[2].setY( bb.bottom());
		pts[3].setX (bb.right() );				pts[3].setY( bb.top());

		painter->drawConvexPolygon (&pts[0], 4);
		break;}

	case	SHP_NOTE:			{
		a = bb.width() / 8;
		pts[0].setX (bb.left() );				pts[0].setY( bb.top());
		pts[1].setX (bb.right()-a );			pts[1].setY( bb.top());
		pts[2].setX (bb.right() );				pts[2].setY( bb.top()+a);
		pts[3].setX (bb.right() );				pts[3].setY( bb.bottom());
		pts[4].setX (bb.left() );				pts[4].setY( bb.bottom());

		painter->drawConvexPolygon (&pts[0], 5);
		pts[3] = pts[2];
		pts[2].setX (pts[1].x()); /* y already == to 3.y) */
		painter->drawPolyline (&pts[1], 3);

		break;}

	case	SHP_PLAINTEXT:
		break;

	case	SHP_PARALLELOGRAM:	{
		a = bb.width() / 8;
		pts[0].setX (bb.left() );				pts[0].setY( bb.bottom());
		pts[1].setX (bb.left()+a );				pts[1].setY( bb.top());
		pts[2].setX (bb.right() );				pts[2].setY( bb.top());
		pts[3].setX (bb.right()-a );			pts[3].setY( bb.bottom());

		painter->drawConvexPolygon (&pts[0], 4);
		break;}


	case	SHP_POLYGON:
	case	SHP_SEPTAGON:		{
		qreal a_1_3 = bb.height() / 3;
		qreal a_2_3 = a_1_3 * 2;

		pts[0].setX (bb.left() );				pts[0].setY( bb.top()+a_2_3);
		pts[1].setX (bb.left()+a_1_3/2 );		pts[1].setY( bb.top()+a_1_3/2);
		pts[2].setX (bb.left()+bb.width()/2 );	pts[2].setY( bb.top());
		pts[3].setX (bb.right()-a_1_3/2 );		pts[3].setY( pts[1].y());
		pts[4].setX (bb.right() );				pts[4].setY( pts[0].y());
		pts[5].setX (bb.right()-a_1_3 );		pts[5].setY( bb.bottom());
		pts[6].setX (bb.left()+a_1_3 );			pts[6].setY( bb.bottom());

		painter->drawConvexPolygon (&pts[0], 7);
		break;}

	case	SHP_POINT:			{
		painter->drawEllipse (bb);
		break;}

	case	SHP_PENTAGON:		{
		a = bb.height() / 3;

		pts[0].setX (bb.left() );				pts[0].setY( bb.top()+a);
		pts[1].setX (bb.left()+bb.width()/2 );	pts[1].setY( bb.top());
		pts[2].setX (bb.right() );				pts[2].setY( bb.top()+a);
		pts[3].setX (bb.right()-a );			pts[3].setY( bb.bottom());
		pts[4].setX (bb.left()+a );				pts[4].setY( bb.bottom());

		painter->drawConvexPolygon (&pts[0], 5);
		break;}

	case	SHP_FOLDER:			{
		qreal a = bb.width() / 8;
		QRectF	bb2 (bb.right()-a*2, bb.top(), a*2, a);
		painter->drawRect (bb);
		painter->drawRect (bb2);

		break;}

	case	SHP_TAB:			{
		qreal a = bb.width() / 8;

		QRectF	bb2 (bb.left(), bb.top(), a*2, a);
		painter->drawRect (bb);
		painter->drawRect (bb2);
		break;}

	default:
		painter->drawRoundedRect (bb, 5, 5);
	}




	/* the label */
	textlabel_t * tl = ND_label (dot_n_);
	QString lbl = tl->text;

	if (tl->html)
	{
		/** @todo	html rendering */
	}
	else
	{
	}

	if (lbl.isEmpty() == false)
	{

		painter->setPen (QColor( tl->fontcolor ).toRgb());

		QFont f = painter->font();
		f.setFamily (tl->fontname);
		f.setPointSize (tl->fontsize);
		painter->setFont (f);


		QTextOption	to;
		to.setWrapMode (QTextOption::WordWrap);
		Qt::AlignmentFlag align;
		switch ( tl->valign )	{
		case	't':
			align = Qt::AlignTop;
			break;
		case	'b':
			align = Qt::AlignBottom;
			break;
		default:
			align = Qt::AlignVCenter;
		}
		/* stuuuuuupid */
		align = (Qt::AlignmentFlag) (0x0004 | align);
		to.setAlignment (align);

		lbl.replace ("\\N", "\n", Qt::CaseInsensitive);
		lbl.replace ("\\T", "\t", Qt::CaseInsensitive);
		lbl.replace ("\\R", "\r", Qt::CaseInsensitive);
		lbl.replace ("\\\\", "\\");

		painter->drawText (bb, lbl, to);

	}


	//	if (atr_label != NULL){

	//		vl = agxget (dot_n_, atr_label->index);

	//		if (atr_fontcolor != NULL){
	//			vl = agxget (dot_n_, atr_fontcolor->index);
	//			painter->setPen (QColor( vl ).toRgb());
	//		}
	//		if (atr_fontname != NULL){
	//			vl = agxget (dot_n_, atr_fontname->index);
	//			QFont f = painter->font();
	//			f.setFamily (vl);
	//			painter->setFont (f);
	//		}
	//		if (atr_fontsize != NULL){
	//			vl = agxget (dot_n_, atr_fontsize->index);
	//			QFont f = painter->font();
	//			a = QString (vl ).toDouble( &b_ok);
	//			if (b_ok && ( a > 1 ))
	//			{
	//				f.setPointSize (a);
	//				painter->setFont (f);
	//			}
	//		}


}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void					GVzNode::updateCachedData			()
{
	if (dot_n_ == NULL)
		return;

	/* position */
	pointf center = ND_coord (dot_n_);
	setPos (center.x, -center.y);

	/* shape */
	shape_desc * shp = ND_shape (dot_n_);
	shp_ = SHP_UDEF;
	if (shp->usershape == false)
	{
		int i;
		int j;
		for ( i = 0; ; i++ )
		{
			if (shape_names[i] == NULL)
			{
				break;
			}
			j = strcmp (shape_names[i], shp->name);
			if (j == 0)
			{
				shp_ = (NdShape)i;
				break;
			}
			else if (j > 0)
			{
				break;
			}
		}
	}

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QList<GVzNode*>			GVzNode::nodes					() const
{
	QList<GVzNode*>	l_nodes;
	GVzEdge *  edg;
	foreach( edg, edg_l_ )	{
		GVzNode * nd = edg->destination();
		if (nd != NULL)
		{
			l_nodes.append (nd);
		}
	}
	return l_nodes;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void					GVzNode::setHighlite		 (bool b_sts)
{
	if (b_sts)
	{
		st_lst_ = (States) (st_lst_ | ST_HIGHLITE);
	}
	else
	{
		st_lst_ = (States) (st_lst_ & (~ST_HIGHLITE));
	}
	update();
}
/* ========================================================================= */

#endif	//	DE_WITH_GRAPHVIZ
/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */

/* graph with color */
/*
digraph		graph_name	{
	node	[
		style = filled
		fillcolor = "#FCE975"
		color ="#ff007f"
		fontcolor="#005500"

	]
	a -> b
}
*/

/* test graph for node shapes */
/*
digraph shapes_test	{
	node_Mcircle [
		shape=Mcircle
		label="Mcircle"
	]

	node_Mdiamond [
		shape=Mdiamond
		label="Mdiamond"
	]

	node_Msquare [
		shape=Msquare
		label="Msquare"
	]

	node_box [
		shape=box
		label="box"
	]

	node_box3d [
		shape=box3d
		label="box3d"
	]

	node_circle [
		shape=circle
		label="circle"
	]

	node_component [
		shape=component
		label="component"
	]

	node_diamond [
		shape=diamond
		label="diamond"
	]

	node_doublecircle [
		shape=doublecircle
		label="doublecircle"
	]

	node_doubleoctagon [
		shape=doubleoctagon
		label="doubleoctagon"
	]

	node_egg [
		shape=egg
		label="egg"
	]

	node_ellipse [
		shape=ellipse
		label="ellipse"
	]

	node_folder [
		shape=folder
		label="folder"
	]

	node_hexagon [
		shape=hexagon
		label="hexagon"
	]

	node_house [
		shape=house
		label="house"
	]

	node_invhouse [
		shape=invhouse
		label="invhouse"
	]

	node_invtrapezium [
		shape=invtrapezium
		label="invtrapezium"
	]

	node_invtriangle [
		shape=invtriangle
		label="invtriangle"
	]

	node_none [
		shape=none
		label="none"
	]

	node_note [
		shape=note
		label="note"
	]

	node_octagon [
		shape=octagon
		label="octagon"
	]

	node_oval [
		shape=oval
		label="oval"
	]

	node_parallelogram [
		shape=parallelogram
		label="parallelogram"
	]

	node_pentagon [
		shape=pentagon
		label="pentagon"
	]

	node_plaintext [
		shape=plaintext
		label="plaintext"
	]

	node_point [
		shape=point
		label="point"
	]

	node_polygon [
		shape=polygon
		label="polygon"
	]

	node_rect [
		shape=rect
		label="rect"
	]

	node_rectangle [
		shape=rectangle
		label="rectangle"
	]

	node_septagon [
		shape=septagon
		label="septagon"
	]

	node_square [
		shape=square
		label="square"
	]

	node_tab [
		shape=tab
		label="tab"
	]

	node_trapezium [
		shape=trapezium
		label="trapezium"
	]

	node_triangle [
		shape=triangle
		label="triangle"
	]

	node_tripleoctagon [
		shape=tripleoctagon
		label="tripleoctagon"
	]

}

*/
