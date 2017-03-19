/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			gvzedge.cc
  \date			May 2012
  \author		TNick

  \brief		Contains the implementation of GVzEdge class


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
#include	<QPainterPath>
#include	<QStyleOptionGraphicsItem>

#include	<impl/gviz/gvznode.h>
#include	"gvzedge.h"


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

/*  DATA    ================================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
GVzEdge::GVzEdge	(
		QGraphicsPathItem * parent, Agedge_t * edge,
		GVzNode * src, GVzNode * dst )
	: QGraphicsPathItem( parent )
{

	setFlags( ItemIsMovable | ItemIsSelectable | ItemIsFocusable);

	dot_e_ = edge;
	updateCachedData();
	src_ = src;
	dst_ = dst;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
GVzEdge::~GVzEdge	()
{

	//	char * shape = agget(abc,"shape");
	//	char * height = agget(abc,"height");
	//	char * width = agget(abc,"width");
	//	char * style = agget(abc,"style");
	//	char * fixedsize = agget(abc,"fixedsize");
	//	char * peripheries = agget(abc,"peripheries");
	//	char * regular = agget(abc,"regular");

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString					GVzEdge::label						() const
{
	if (dot_e_ == NULL)
		return QString();

	textlabel_t * tl = ED_label (dot_e_);
	if (tl == NULL)
		return QString();

	return tl->text;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void		addtxToPath ( textlabel_t * tl, QPainterPath & big_pth )
{
	if (tl == NULL)
		return;

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

		QFont f;
		f.setFamily (tl->fontname);
		f.setPointSize (tl->fontsize);

		lbl.replace ("\\N", "\n", Qt::CaseInsensitive);
		lbl.replace ("\\T", "\t", Qt::CaseInsensitive);
		lbl.replace ("\\R", "\r", Qt::CaseInsensitive);
		lbl.replace ("\\\\", "\\");

		big_pth.addText (tl->pos.x, -tl->pos.y, f, lbl);
	}

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void					GVzEdge::updateCachedData			()
{/*https://code.monotone.ca/p/guitone/source/file/ca594791cdfb69f506df7a96d3b9411a3ed72616/src/model/GraphScene.cpp*/
#define	q_agfindattr(a,b)	agfindattr( a, const_cast<char *>( b ) )

	if (dot_e_ == NULL)
		return;

	char * vl;
	Agsym_t * atr_clr = q_agfindattr (dot_e_, "color");
	Agsym_t * atr_fillclr = q_agfindattr (dot_e_, "fillcolor");
	Agsym_t * atr_style = q_agfindattr (dot_e_, "style");
	Agsym_t * atr_ahead = q_agfindattr (dot_e_, "arrowhead");
	Agsym_t * atr_asize = q_agfindattr (dot_e_, "arrowsize");
	Agsym_t * atr_atail = q_agfindattr (dot_e_, "arrowtail");
	Agsym_t * atr_dir = q_agfindattr (dot_e_, "dir");
//	compound, len, lhead, ltail, minlen, nodesep, pin, pos, rank,
//	rankdir, ranksep and weight.

	/*
	Name			Default				Use
	arrowhead		normal				style of arrowhead at head end
	arrowsize		1.0					scaling factor for arrowheads
	arrowtail		normal				style of arrowhead at tail end
	color			black				edge stroke color
	decorate							if set, draws a line connecting labels with their edges
	dir				forward/none		forward, back, both, or none
	fontcolor		black				type face color
	headlabel							label placed near head of edge
	labelangle		-25.0				angle in degrees which head or tail label is rotated off edge
	labeldistance	1.0					scaling factor for distance of head or tail label from node
	labelfloat		false				lessen constraints on edge label placement
	labelfontcolor	black				type face color for head and tail labels
	labelfontname	Times-Roman			font family for head and tail labels
	labelfontsize	14					point size for head and tail labels
	layer			overlay range		all, id or id:id
	nojustify		false				context for justifying multiple lines of text
	style								drawing attributes such as bold, dotted, or	filled
	taillabel							label placed near tail of edge
	*/



	if (atr_clr != NULL){
		vl = agxget (dot_e_, atr_clr->index);
		setPen (QColor( vl ).toRgb());
	}
	if (atr_fillclr != NULL){
		vl = agxget (dot_e_, atr_fillclr->index);
		setBrush (QColor( vl ).toRgb());
	}
	if (atr_style != NULL){
		vl = agxget (dot_e_, atr_style->index);
		//painter->setPen (QColor( vl ).toRgb());
	}
	if (atr_ahead != NULL){
		vl = agxget (dot_e_, atr_ahead->index);
		//painter->setPen (QColor( vl ).toRgb());
	}
	if (atr_asize != NULL){
		vl = agxget (dot_e_, atr_asize->index);
		//painter->setPen (QColor( vl ).toRgb());
	}
	if (atr_atail != NULL){
		vl = agxget (dot_e_, atr_atail->index);
		//painter->setPen (QColor( vl ).toRgb());
	}
	if (atr_dir != NULL){
		vl = agxget (dot_e_, atr_dir->index);
		//painter->setPen (QColor( vl ).toRgb());
	}

	QPainterPath big_pth;

	/* the labels */
	addtxToPath (ED_label( dot_e_ ), big_pth);
	addtxToPath (ED_head_label( dot_e_ ), big_pth);
	addtxToPath (ED_tail_label( dot_e_ ), big_pth);


	splines *	sp_lst = ED_spl (dot_e_);
	//	typedef struct splines {
	//		bezier *list;
	//		int size;
	//		boxf bb;
	//	} splines;

	for (int i = 0 ; i < sp_lst->size; ++i)
	{
		//bezier * bz_l = sp_lst->list[i];
		//	typedef struct bezier {
		//		pointf *list;
		//		int size;
		//		int sflag, eflag;
		//		pointf sp, ep;
		//	} bezier;

		if (sp_lst->list[i].size < 1)
			continue;
		pointf * points = sp_lst->list[i].list;

		if (sp_lst->list[i].sflag)
		{ /* there is an arrow at head */
			/* the point bz_l->sp is the tip of the arrow */

			pointf base = points[0];
			base.y *= -1;
			pointf head = sp_lst->list[i].sp;
			head.y *= -1;

			QPolygonF arrow;
			arrow.append(
						QPointF(head.x, head.y)
						);
			arrow.append(
						QPointF(base.x + (base.y - head.y) / 2,
								 base.y + (head.x - base.x) / 2)
						);
			arrow.append(
						QPointF(base.x + (head.y - base.y) / 2,
								 base.y + (base.x - head.x) / 2)
						);
			arrow.append(
						QPointF(head.x, head.y)
						);

			big_pth.addPolygon(arrow);

		}

		int j_lim = sp_lst->list[i].size;
		int j = 0;
		for ( ; j+3 < j_lim; j += 3 )
		{
			big_pth.moveTo (
						points[j].x,
						-points[j].y
						);
			big_pth.cubicTo (
						points[j+1].x,
						-points[j+1].y,
						points[j+2].x,
						-points[j+2].y,
						points[j+3].x,
						-points[j+3].y
						);
		}

		if (sp_lst->list[i].eflag)
		{ /* there is an arrow at tail */
			/* the point sp_lst->list[i].ep is the tip of the arrow */

			pointf base = points[j];
			base.y *= -1;
			pointf head = sp_lst->list[i].ep;
			head.y *= -1;

			QPolygonF arrow;
			arrow.append(
						QPointF(head.x, head.y)
						);
			arrow.append(
						QPointF(base.x + (base.y - head.y) / 2,
								 base.y + (head.x - base.x) / 2)
						);
			arrow.append(
						QPointF(base.x + (head.y - base.y) / 2,
								 base.y + (base.x - head.x) / 2)
						);
			arrow.append(
						QPointF(head.x, head.y)
						);

			big_pth.addPolygon(arrow);

		}




	}
	//Agedgeinfo_t
	//ED_tail_label

	/* see page 18 */

	setPath (big_pth);

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
