/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dfautomata.cc
  \date			May 2012
  \author		TNick

  \brief		Contains the implementation of DFAutomata class


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

#include	<dot-editor/dot-editor.h>

#include	<dot-editor/usermsg.h>

#include	<dot-editor/impl/implementations/implgvizlib.h>
#include	<dot-editor/impl/gviz/gvzedge.h>
#include	<dot-editor/impl/gviz/gvznode.h>

#include	<dot-editor/plugins/automata/automatacomobj.h>
#include	<dot-editor/plugins/automata/de-plgin-automata.h>

#include	"dfautomata.h"


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
DFAutomata::DFAutomata	( PlugIns::Automata_PlugIn * plg_inst )
	: CmnAutomImpl( plg_inst )
{
	crt_n_ = NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
DFAutomata::~DFAutomata	( void )
{
	if ( crt_n_ !=  NULL )
	{
		/** @todo	deselect the node */

	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				DFAutomata::reset	( void )
{
	crt_n_ = NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				DFAutomata::goStep				( const QString & s_act )
{
	UserMsg	um;

#ifdef	DE_WITH_GRAPHVIZ
    if ( crt_n_ ==  NULL )
	{ /* we need to find first node */
		crt_n_ = locateStartNode( um );
		if ( crt_n_ ==  NULL )
		{
			um.show();
			return;
		}
		plg_->appendStep( QString(), crt_n_, 0 );
	}
	else
	{
		crt_n_->setHighlite( false );
	}

	/* get the list of edges that leave this node */
	QList<GVzEdge*>		edg_l = crt_n_->edges();
	GVzEdge * edg;
	foreach( edg, edg_l )
	{
		if ( edg->label().compare( s_act, Qt::CaseInsensitive ) == 0 )
		{ /* we have found the edge */
			crt_n_ = edg->destination();
			if ( crt_n_ == NULL )
				break;
			plg_->appendStep( s_act, crt_n_, 0 );
			crt_n_->setHighlite( true );
			return;
		}
	}
	plg_->appendStep( s_act, NULL, 0 );
	crt_n_->setHighlite( false );
	crt_n_ = NULL;
	return;
#endif // DE_WITH_GRAPHVIZ

}
/* ========================================================================= */

/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
