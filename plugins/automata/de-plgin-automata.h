/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			de-plgin-automata.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for AboutDlg class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __AUTOMATA_PLUGIN_INC__
#define __AUTOMATA_PLUGIN_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */
#include	<dot-editor/dot-editor.h>

#include	<dot-editor/plugin.h>


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

class		QToolBar;
class		QDockWidget;
class		QPlainTextEdit;
class		QTimer;

class		UserMsg;
class		CmnAutomImpl;
class		GVzNode;


namespace	PlugIns		{

class		AutomataComObj;

/// The plug-in implementing Automata
/**
*	All plug-ins must inherit from PlugIn class
*/
class Automata_PlugIn	: public	PlugIn			{

	//
	//
	//
	//
	/*  DEFINITIONS    ----------------------------------------------------- */

#	define	DE_PLGIN_AUTOMATA		"Automata"
#	define	DE_PLGIN_STG			"plgin/" DE_PLGIN_AUTOMATA

	friend class AutomataComObj;


	/**
	*	@brief	plug-in related status
	*/
	enum	AtmStatus			{

		STS_IDLE,		/**< nothing really interesting */
		STS_RUN,		/**< running the commands without a break */
		STS_STEPPING,	/**< performing a step */
		STS_PAUSE,		/**< paused after performing a step */

		STS_MAX			/**< validation */
	};



	/**
	*	@brief	index of actions in the toolbar
	*/
	enum	ActIdx			{

		TGL_IMP_TYPE,	/**< change input type (how we interpret strings) */
		TGL_SEP_1,		/**< separator */
		RUN_AUTO,		/**< runs the program in input window */
		RUN_STEP,		/**< runs the program and pauses after each command */
		FORCE_STOP,		/**< stops the execution */

		ACT_IDX_MAX		/**< validation */
	};


public:

	/**
	*	@brief	the way we interpret input
	*/
	enum	ImpType		{

		/**
		*	@brief	each character represents a command; white spaces separate them
		*
		*	Below are two command sequences. Each character in the string is
		*	a command to be interpretted. White spaces (space, tab, new lines)
		*	separates command sequences.
		*
		*	@code
		*	010111 465877
		*	@endcode
		*/
		IMP_CHARACTERS,


		/**
		*	@brief	each string represents a command; new lines separate them
		*
		*	Below there are two command sequences. Each string represents
		*	a command (a string is either delimited by white spaces or - if it
		*	contains white spaces - delimited by quotation marks).
		*
		*	@code
		*	first_command_seq_1
		*		"second command"
		*		third fourth
		*
		*	fifth
		*
		*
		*	# two consecutive new lines start a new command
		*	second_command_seq_starts_here
		*	a b c
		*
		*	@endcode
		*/
		IMP_STRINGS,


		/**
		*	@brief	validation
		*/
		IMP_MAX
	};



	/*  DEFINITIONS    ===================================================== */
	//
	//
	//
	//
	/*  DATA    ------------------------------------------------------------ */

private:

	/**
	*	@brief	associated toolbar created when the plug-in is loaded
	*/
	QToolBar *			tb_;


	/**
	*	@brief	associated tool-window created when the plug-in is loaded
	*/
	QDockWidget *		dk_;


	/**
	*	@brief	Input text panel; gives the string to execute
	*/
	QPlainTextEdit *	tx_inp_;


	/**
	*	@brief	Result text panel; shows the trace
	*/
	QPlainTextEdit *	tx_res_;


	/**
	*	@brief	timer that helps us perform repetitive tasks
	*/
	QTimer *			timer_;


	/**
	*	@brief	communication class
	*/
	AutomataComObj *	cmo_;


	/**
	*	@brief	the way we're interpretting strings
	*/
	ImpType				md_;


	/**
	*	@brief	what is the plug-in doing right now
	*/
	AtmStatus			sts_;


	/**
	*	@brief	the automata that we're using right now
	*/
	CmnAutomImpl *		a_obj_;


	/*  DATA    ============================================================ */
	//
	//
	//
	//
	/*  FUNCTIONS    ------------------------------------------------------- */

public:

	/**
	*	@brief	constructor
	*/
	Automata_PlugIn			(void);


	/**
	*	@brief	destructor
	*/
	~Automata_PlugIn		(void);



	/**
	*	@brief	change the way command strings are interpretted (characters / strings)
	*/
	void			setDefaultInterpMode	( ImpType md );


	/**
	*	@brief	stops the execution of the program
	*/
	void			stopAutomProg			( void );


	/**
	*	@brief	strarts the execution of the program
	*/
	void			runAutomProg			( void );


	/**
	*	@brief	starts the execution of the program in step mode
	*/
	void			stepAutomProg			( void );


	/**
	*	@brief	executes specified number of steps
	*
	*	If the \b i_cnt member is 0 or less than 0 the settings are querried
	*	for the number of steps to perform.
	*
	*	This method alters current state. If there are steps left then it sets
	*	the state to previous state, otherwise it stops the timer and sets the
	*	state to STS_IDLE.
	*/
	void			performSteps			( int i_cnt = 1 );


	/**
	*	@brief	appends a node and path to specified line
	*/
	void				appendStep			(
			const QString &				s_pth,
			const GVzNode *				nd_next,
			int							i
			);



protected:


	/**
	*	@brief	toolbar was closed by the GUI
	*/
	inline void		noTb					( void )
	{ tb_ = NULL; }


	/**
	*	@brief	tool-window was closed by the GUI
	*/
	inline void		noTw					( void )
	{ dk_ = NULL; }


	/**
	*	@brief	informed when the text has changed
	*/
	void			editorChanged			( void );


private:


	/**
	*	@brief	create toolbar (only once when plug-in starts)
	*/
	void			createAssocTb			( void );

	/**
	*	@brief	create tool-window (only once when plug-in starts)
	*/
	void			createAssocTw			( void );


	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class Automata_PlugIn	*/


PLGIN_EXPORT PlugIns::PlugIn *		getPlugInInterface			(
		char * p_version,
		UserMsg & um
		);


/*  CLASS    =============================================================== */
//
//
//
//

}	//	namespace PlugIns

#endif // __AUTOMATA_PLUGIN_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
