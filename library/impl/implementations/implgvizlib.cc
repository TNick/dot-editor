/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			implgvizlib.cc
  \date			May 2012
  \author		TNick

  \brief		Contains the implementation of ImplGVizLib class


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

#include	<QSettings>
#include	<QDebug>

#include	<impl/gviz/gvzedge.h>
#include	<impl/gviz/gvznode.h>

#include	<gui/mw.h>
#include	<gui/guictrl.h>

#include	"implgvizlib.h"


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

ImplGVizLib *		ImplGVizLib::uniq_ = NULL;

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
ImplGVizLib::ImplGVizLib	()
	: ImplInterf()
{
	Q_ASSERT (uniq_ == NULL);
	uniq_ = this;

	s_.load();
	applySettings();

	context_ = gvContext();
	graph_ = NULL;

	Q_ASSERT (context_ != NULL);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ImplGVizLib::~ImplGVizLib	()
{
	s_.save();

	if (graph_ != NULL)
	{
		gvFreeLayout( context_, graph_);
		agclose (graph_);
	}

	gvFreeContext (context_);

	Q_ASSERT (uniq_ == this);
	uniq_ = NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplGVizLib::Stg::save			()
{
	QSettings stgs;
	Q_UNUSED (stgs);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplGVizLib::Stg::load			()
{
	QSettings stgs;
	Q_UNUSED (stgs);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
*	@brief Add an alternative value parameter to the method for getting an
*	object's attribute
*
*	http://mupuf.org/blog/article/34/
*/
static inline QString		_agget			(
		void * object, const QString attr, const QString alt = QString() )
{
	QString str = agget(
				object,
				const_cast<char *>( qPrintable( attr ) )
				);

	if( str.isEmpty() )
		return alt;
	else
		return str;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
*	@brief	Directly use agsafeset which always works, contrarily to agset
*
*	http://mupuf.org/blog/article/34/
*/
static inline int			_agset			(
		void * object, const QString & attr, const QString & value )
{
	return agsafeset(
				object,
				const_cast<char *>( qPrintable(attr) ),
				const_cast<char *>( qPrintable(value) ),
				const_cast<char *>( qPrintable(value) )
				);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool				ImplGVizLib::perform			()
{
	QString s = GuiCtrl::editorText().trimmed();

	/* clear the scene */
	img_scene.clear();
	if (graph_ != NULL)
	{
		gvFreeLayout( context_, graph_);
		agclose (graph_);
		graph_ = NULL;
	}

	/* don't bother if there's nothing to do */
	if (s.isEmpty())
	{
		return true;
	}

	/* get dot info about this graph */
	graph_ = agmemread (const_cast<char*>( qPrintable( s ) ));
	if (graph_ == NULL)
		return false;

	gvLayout (context_, graph_, "dot");

	Agnode_t * n;
	Agedge_t *e;
	GVzNode * nd;
	GVzEdge * edg;
	QList<GVzEdge*>	edges;
	QList<GVzNode*>	nodes;
	bool	b_sub_n;

	/* remove the result of previous pass */
	tl_nodes_.clear();

	/* iterate in nodes created now */
	n = agfstnode (graph_);
	//for (n = agfstnode (graph_); n; n = agnxtnode( graph_, n ) )
	while( n != NULL )
	{
		nd = new GVzNode (NULL, n, NULL);
		nodes.append (nd);
		img_scene.addItem (nd);
		b_sub_n = false;
		qDebug() << nd->label();
		qDebug() << "\t from node " << (void*)n;

		foreach( edg, edges )
		{

			if (edg->dot_e_->head == n)
			{
				edges.removeOne (edg);
				edg->setDestination (nd);
				nd->setParentNode (edg->source());
				b_sub_n = true;
				qDebug() << "\t appended to " << (void*)edg;
				break;
			}
		}
		if (b_sub_n == false)
		{
			tl_nodes_.append (nd);
		}

		for  (e = agfstout( graph_, n); e; e = agnxtout( graph_, e ) )
		{
			edg = new GVzEdge (NULL, e, nd, NULL);
			img_scene.addItem (edg);
			nd->appendEdge (edg);
			edges.append (edg);
			qDebug() << "\t edge" << (void*)e << "  " << (void*)edg
					 << "\t head " << e->head << "\t tail " << e->tail;

		}

		n = agnxtnode (graph_, n);
	}

	foreach( edg, edges )
	{
		qDebug() << "Left in l: " << (void*)edg;
		foreach( nd, nodes )
		{
			if (edg->dot_e_->head == nd->dot_n_)
			{
				edg->setDestination (nd);
				nd->setParentNode (edg->source());
				qDebug() << "Found: " << (void*)edg << " " << (void*)nd;
			}
		}
	}

	return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplGVizLib::ctxDisplay			(
		const QPoint & loc, const QPoint & glob_loc )
{
	Q_UNUSED (loc);
	Q_UNUSED (glob_loc);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ImplGVizLib::Stg	ImplGVizLib::settings			()
{
	if (uniq_ == NULL)
	{
		Stg	s_ret;
		s_ret.load();
		return s_ret;
	}
	else
	{
		return uniq_->s_;
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplGVizLib::setSettings	 (ImplGVizLib::Stg & stg)
{
	if (uniq_ == NULL)
	{
		stg.save();
	}
	else
	{
		uniq_->s_ = stg;
		uniq_->applySettings();
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplGVizLib::applySettings		()
{

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QAction *			ImplGVizLib::action				()
{
	return Gui::MW::unique()->predefAct_GViz();
}
/* ========================================================================= */

#endif	//	DE_WITH_GRAPHVIZ//
/*  CLASS    =============================================================== */
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
