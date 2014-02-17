/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			de-plgin-automata.cc
  \date			Mar 2012
  \author		TNick

  \brief		Contains the definition for AboutDlg class


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

#include	<QDebug>
#include	<QSettings>
#include	<QVariant>
#include	<QToolBar>
#include	<QAction>
#include	<QDockWidget>
#include	<QSplitter>
#include	<QPlainTextEdit>
#include	<QVBoxLayout>
#include	<QLabel>
#include	<QToolButton>
#include	<QTimer>


#include	<dot-editor/usermsg.h>
#include	<dot-editor/plginman.h>

#include	<dot-editor/impl/implinterf.h>
#include	<dot-editor/impl/implmanager.h>
#include	<dot-editor/impl/implementations/implgvizlib.h>
#include	<dot-editor/impl/gviz/gvzedge.h>
#include	<dot-editor/impl/gviz/gvznode.h>

#include	<dot-editor/gui/guictrl.h>

#include	<dot-editor/plugins/automata/dfautomata.h>
#include	<dot-editor/plugins/automata/nfautomata.h>
#include	<dot-editor/plugins/automata/enfautomata.h>

#include	"automatacomobj.h"
#include	"de-plgin-automata.h"


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
extern "C"	{
PlugIns::PlugIn *		getPlugInInterface			(
		char * p_version, UserMsg & um )
{
	Q_UNUSED( p_version );

	um.add( UserMsg::MSG_INFO,
			AutomataComObj::tr( "The " DE_PLGIN_AUTOMATA " plugin was loaded" )
			);

	qDebug() << "The " DE_PLGIN_AUTOMATA "plugin was loaded";
	return new Automata_PlugIn();
}
}
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
Automata_PlugIn::Automata_PlugIn	(void)
	: PlugIn()
{
	QSettings	stg;
	bool b_ok = false;
	int i;

	sts_ = STS_IDLE;
	setName( DE_PLGIN_AUTOMATA );

	if ( ImplManager::hasGViz() == false )
	{
		UserMsg	um;
		um.add( UserMsg::MSG_ERROR,
				AutomataComObj::tr(
					"The binary is compiled without GraphViz support.\n"
					"Automata plug-in can't run without it." )
				);
		um.show();
		tb_ = NULL;
		dk_ = NULL;
		cmo_ = NULL;
		timer_ = NULL;
		return;
	}

	cmo_ = new AutomataComObj( this );
	createAssocTb();
	createAssocTw();

	/* create and set-up the timer */
	timer_ = new QTimer();
	i = stg.value( DE_PLGIN_STG "/timerStep", 500).toInt( &b_ok );
	if ( b_ok == false )
		i = 500;
	timer_->setInterval( i );
	AutomataComObj::connect(
				timer_, SIGNAL( timeout() ),
				cmo_, SLOT( timeoutRun() )
				);


	setDefaultInterpMode( (ImpType)stg.value(
							  DE_PLGIN_STG "/interpMode",
							  IMP_STRINGS
							  ).toInt() );


	connectTo_TxChanged( cmo_, SLOT( editorChanged() ) );

	a_obj_ = new DFAutomata( this );

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Automata_PlugIn::~Automata_PlugIn	(void)
{
	QSettings	stg;


//	if ( tb_ != NULL )
//	{
//		tb_->close();
//		tb_->deleteLater();
//		tb_ = NULL;
//	}
//	if ( dk_ != NULL )
//	{
//		dk_->close();
//		dk_->deleteLater();
//		dk_ = NULL;
//	}

	stg.setValue( DE_PLGIN_STG "/interpMode", md_ );
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			Automata_PlugIn::performSteps			( int i_cnt )
{
	int i;
	AtmStatus	entry_sts = sts_;
	sts_ = STS_STEPPING;

	/* find out how many times we're doing this */
	if ( i_cnt < 1 )
	{
		QSettings	stg;
		bool b_ok = false;
		i_cnt = stg.value( DE_PLGIN_STG "/timerStep", 1).toInt( &b_ok );
		if ( b_ok == false )
			i_cnt = 1;
	}

	QList<QTextEdit::ExtraSelection> extra_s = tx_inp_->extraSelections();
	QTextEdit::ExtraSelection es;
	if ( extra_s.count() < 1 )
	{
		/* we're just getting started */
		es.cursor = tx_inp_->textCursor();
		es.cursor.setPosition( 0 );
		es.format.setBackground( QBrush( Qt::red ) );
		es.format.setFontStrikeOut( true );

		extra_s.append( es );
	}
	es = extra_s.at( 0 );
	QTextCursor * tc = &es.cursor;

	/*
	 * 0 - between strings
	 * 1 - in string without quote
	 * 2 - in string with single quote
	 * 3 - in string with double quote
	 */
	int s_step = 0;
	int s_start = -1;

	for ( i = 0; i < i_cnt; i++ )
	{
		/* let us skip white spaces without counting */
		for ( ;; )		{

			/* check for the end of the input */
			if ( tc->atEnd() )
			{
				if ( md_ == IMP_STRINGS )
				{
					if ( s_step != 0 ) /* between strings */
					{
						QString	s = tc->selectedText();
						a_obj_->goStep( s.mid( s_start ) );
						break;
					}
				}

				timer_->stop();
				sts_ = STS_IDLE;
				extra_s.clear();
				tx_inp_->setExtraSelections( extra_s );
				return;
			}

			/* the position is always after an executable element */

			/* incorporate a new character in selection */
			tc->setPosition( tc->position() + 1, QTextCursor::KeepAnchor );
			QString	s = tc->selectedText();
			QChar c = s.at( s.length() - 1 );
			Q_UNUSED( c );

			/* different run modes => different interpretations */
			if ( md_ == IMP_CHARACTERS )
			{
				if ( c.isSpace() )
				{
					/** @todo a program end here? */
				}
				else
				{
					a_obj_->goStep( c );
					break;
				}
			}
			else if ( md_ == IMP_STRINGS )
			{
				if ( s_step == 0 ) /* between strings */
				{
					if ( c.isSpace() )
					{ /* still between elements */
						continue;
					}
					else if ( c == '"' )
					{ /* starting a quoted string */
						s_step = 3;
						s_start = s.length();
					}
					else if ( c == '\'' )
					{ /* starting a quoted string */
						s_step = 2;
						s_start = s.length();
					}
					else
					{ /* in unquoted string */
						s_step = 1;
						s_start = s.length() - 1;
					}
					s_start = s.length();
					continue;
				}
				else if ( s_step == 1 ) /* in string without quote */
				{
					if ( c.isSpace() )
					{ /* found the end */
						a_obj_->goStep( s.mid( s_start ) );
						break;
					}
				}
				else if ( s_step == 2 ) /* in string with single quote */
				{
					if ( c == '\'' )
					{ /* found the end */
						a_obj_->goStep( s.mid( s_start ) );
						break;
					}
				}
				else if ( s_step == 3 ) /* in string with double quote */
				{
					if ( c == '"' )
					{ /* found the end */
						a_obj_->goStep( s.mid( s_start ) );
						break;
					}
				}
				else
				{
					Q_ASSERT( false );
				}
			}
			else /* md_ == ... */
			{
				Q_ASSERT( false );
			}
		} /* for ( ;; ) */

		if ( sts_ == STS_IDLE )
		{
			entry_sts = STS_IDLE;
			break;
		}
	} /* for ( i = 0; i < i_cnt; i++ ) */

	sts_ = entry_sts;
	extra_s[0] = es;
	tx_inp_->setExtraSelections( extra_s );

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			Automata_PlugIn::setDefaultInterpMode	( ImpType md )
{
	md_ = md;
	if ( tb_ != NULL )
	{
		QAction * ac = tb_->actions().at( TGL_IMP_TYPE );
		ac->setChecked( (md == IMP_STRINGS) );
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			Automata_PlugIn::createAssocTw			( void )
{
	dk_ = GuiCtrl::addDock( AutomataComObj::tr( "Automata" ) );
	dk_->setAllowedAreas( Qt::AllDockWidgetAreas );

	QWidget * m_wdg = new QWidget( dk_ );
	QVBoxLayout * vbl = new QVBoxLayout( m_wdg );
	m_wdg->setLayout( vbl );

	tb_->setParent( m_wdg );
	vbl->addWidget( tb_ );

	QLabel * lbl = new QLabel(
				AutomataComObj::tr( "Enter the text to execute below" ),
				m_wdg
				);
	vbl->addWidget( lbl );
	QSplitter * spl = new QSplitter( Qt::Vertical, dk_ );
	vbl->addWidget( spl );

	tx_inp_ = new QPlainTextEdit( m_wdg );
	tx_res_ = new QPlainTextEdit( m_wdg );
	tx_res_->setWordWrapMode( QTextOption::NoWrap );
	//tx_res_->setReadOnly( true );

	spl->addWidget( tx_inp_ );
	spl->addWidget( tx_res_ );
	dk_->setWidget( m_wdg );

	AutomataComObj::connect(
				dk_, SIGNAL( destroyed() ),
				cmo_, SLOT( twClosing() )
				);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			Automata_PlugIn::createAssocTb			( void )
{
	QSettings	stg;
	QAction *	act;


	/* prepare the tool-bar */
	tb_ = createTb(
				(Qt::ToolBarArea)stg.value(
					DE_PLGIN_STG "/tbLoc",
					Qt::TopToolBarArea
					).toInt(),
				DE_PLGIN_AUTOMATA
				);
	tb_->setObjectName( DE_PLGIN_AUTOMATA );

	/* TGL_IMP_TYPE - change input type (how we interpret strings) */
	act = tb_->addAction(
				QIcon( ":/resources/images/str-change.png" ),
				AutomataComObj::tr( "Change input type" ),
				cmo_,
				SLOT(setDefaultInterpMode(bool))
				);
	act->setCheckable( true );
	act->setToolTip(
				AutomataComObj::tr(
					"Changes the way we interpret strings. If on (pushed),\n"
					"each string is a command. If off, each character\n"
					"is a command."
					)
				);

	tb_->addSeparator();

	/* RUN_AUTO - runs the program in input window */
	act = tb_->addAction(
				QIcon( ":/resources/images/run.png" ),
				AutomataComObj::tr( "Run" ),
				cmo_,
				SLOT(runAutomProg())
				);
	act->setToolTip(
				AutomataComObj::tr(
					"Runs the program in command panel.\n"
					"The program will run all command sequences.\n"
					"To force it to stop use Stop button."
					)
				);


	/* RUN_STEP - runs the program and pauses after each command */
	act = tb_->addAction(
				QIcon( ":/resources/images/single-step.png" ),
				AutomataComObj::tr( "Step" ),
				cmo_,
				SLOT(stepAutomProg())
				);
	act->setToolTip(
				AutomataComObj::tr(
					"Runs the program in command panel.\n"
					"The program will execute a command at a time.\n"
					"To stop it use the Stop button."
					)
				);


	/* FORCE_STOP - stops the execution */
	act = tb_->addAction(
				QIcon( ":/resources/images/stop.png" ),
				AutomataComObj::tr( "Stop" ),
				cmo_,
				SLOT(stopAutomProg())
				);
	act->setToolTip(
				AutomataComObj::tr(
					"Stops the execution of the program."
					)
				);
	//act->setEnabled( false );

	AutomataComObj::connect(
				tb_, SIGNAL( destroyed() ),
				cmo_, SLOT( tbClosing() )
				);

	tb_->setVisible( true );
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			Automata_PlugIn::stopAutomProg			( void )
{
	switch( sts_ )	{
	case	STS_RUN:
	case	STS_STEPPING:
	case	STS_PAUSE:	{
		timer_->stop();
		sts_ = STS_IDLE;
		/* remove visual marker */
		QList<QTextEdit::ExtraSelection> extra_s;
		tx_inp_->setExtraSelections( extra_s );
		a_obj_->reset();

		return;
	}
	default:
#ifdef QT_NO_DEBUG
		qDebug() << "stopAutomProg() with bad state: " << sts_;
#endif
		return;
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			Automata_PlugIn::runAutomProg			( void )
{
	switch( sts_ )	{
	case	STS_STEPPING:
	case	STS_PAUSE:
	case	STS_IDLE:	{
		a_obj_->reset();
		sts_ = STS_RUN;
		/* make sure we're starting fresh */
		QList<QTextEdit::ExtraSelection> extra_s;
		tx_inp_->setExtraSelections( extra_s );
		/* perform a step now */
		performSteps();
		/* and continue */
		timer_->start();
		return;
	}
	default:
#ifdef QT_NO_DEBUG
		qDebug() << "runAutomProg() with bad state: " << sts_;
#endif
		return;
	}

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			Automata_PlugIn::stepAutomProg			( void )
{
	switch( sts_ )	{
	case	STS_IDLE:
	case	STS_PAUSE:
		sts_ = STS_PAUSE;
		/* perform a step now */
		performSteps();
		return;
	default:
#ifdef QT_NO_DEBUG
		qDebug() << "stepAutomProg() with bad state: " << sts_;
#endif
		return;
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			Automata_PlugIn::editorChanged			( void )
{
	Q_ASSERT( a_obj_ != NULL );
	a_obj_->reset();
}
/* ========================================================================= */
#include	<QTextDocument>
#include	<QTextBlock>
#include	<QTextCursor>
#include	<QTextCharFormat>


/* ------------------------------------------------------------------------- */
void			Automata_PlugIn::appendStep					(
		const QString & s_pth, const GVzNode * nd_next, int i )
{
	QTextCharFormat tcf_y;
	QTextCharFormat tcf_w;
	tcf_y.setBackground( QBrush( Qt::yellow ) );
	tcf_w.setBackground( QBrush( Qt::white ) );

	QTextDocument * td = tx_res_->document();
	Q_ASSERT( td != NULL );

	QTextBlock tb;

	for( ;; )
	{
		tb = td->findBlockByLineNumber( i );
		if ( tb.isValid() )
			break;
		tx_res_->appendPlainText( "\n" );
	}
	QTextCursor tc( tb );
	tc.movePosition( QTextCursor::EndOfBlock );


	if ( s_pth.isEmpty() == false )
	{
		tc.insertText( "-" + s_pth + "->", tcf_y );
	}
	if ( nd_next == NULL )
	{
		tc.insertText( "(die)", tcf_w );
	}
	else
	{
#ifdef	DE_WITH_GRAPHVIZ
        tc.insertText( nd_next->label(), tcf_w );
#endif // DE_WITH_GRAPHVIZ
    }

}
/* ========================================================================= */


/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
