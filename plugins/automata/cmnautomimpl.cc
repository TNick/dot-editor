/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			cmnautomimpl.cc
  \date			May 2012
  \author		TNick

  \brief		Contains the implementation of CmnAutomImpl class


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

#include	"cmnautomimpl.h"


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

using namespace PlugIns;

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
CmnAutomImpl::CmnAutomImpl	( PlugIns::Automata_PlugIn * plg_inst )
{
	plg_ = plg_inst;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
CmnAutomImpl::~CmnAutomImpl	( void )
{
	/* stub */
}
/* ========================================================================= */



/* ------------------------------------------------------------------------- */
GVzNode *			CmnAutomImpl::locateStartNode		( UserMsg & um )
{
	GVzNode * ret_n = NULL;
#ifdef	DE_WITH_GRAPHVIZ
    bool	b;

	ImplGVizLib * igv = ImplGVizLib::unique( false );
	if ( igv == NULL )
	{
		um.add( UserMsg::MSG_ERROR, AutomataComObj::tr(
					"Automata plug-in can be used only with GViz back-end." ) );
		return NULL;
	}

	/* get first top-level node */
	QList<GVzNode *> hn = igv->headNodes();
	if ( hn.length() == 0 )
	{
		um.add( UserMsg::MSG_ERROR, AutomataComObj::tr(
					"No top level node to start with." ) );
		return NULL;
	}
	else if ( hn.length() > 1 )
	{
		um.add( UserMsg::MSG_WARNING, AutomataComObj::tr(
					"Multiple top level nodes." ) );
		b = false;
		foreach( ret_n, hn )
		{
			if ( ret_n->nodeShape() == GVzNode::SHP_POINT )
			{
				b = true;
				break;
			}
			else if ( ret_n->label().compare( "start", Qt::CaseInsensitive ) == 0 )
			{
				b = true;
				break;
			}
		}
		if ( b == false )
		{
			/* simply pick first */
			ret_n = hn.at( 0 );
		}
		else
		{
			/* if the node that we have found i simply a marker, get next node */
			QList<GVzNode*> nd_out = ret_n->nodes();
			if ( nd_out.count() == 1 )
			{
				ret_n = nd_out.at( 0 );
			}
		}
	}
	else
	{
		/* only one, things are clear */
		ret_n = hn.at( 0 );
	}

#endif // DE_WITH_GRAPHVIZ
    return ret_n;
}
/* ========================================================================= */


/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
