/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			automatacomobj.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for AutomataComObj class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __AUTOMATACOMOBJ_INC__
#define __AUTOMATACOMOBJ_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<QObject>

#include	<dot-editor/plugins/automata/de-plgin-automata.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

namespace	PlugIns		{

/**
*	@brief	Communication object used by the Automata_PlugIn class
*/
class AutomataComObj	: public QObject			{
    Q_OBJECT

    //
    //
    //
    //
    /*  DEFINITIONS    ----------------------------------------------------- */

    friend class Automata_PlugIn;

    /*  DEFINITIONS    ===================================================== */
    //
    //
    //
    //
    /*  DATA    ------------------------------------------------------------ */

private:


    /**
    *	@brief	associated plug-in
    */
    Automata_PlugIn *		parent_;


    /*  DATA    ============================================================ */
    //
    //
    //
    //
    /*  FUNCTIONS    ------------------------------------------------------- */

public:


    /**
    *	@brief	constructor;
    */
    AutomataComObj			( Automata_PlugIn * p );


    /**
    *	@brief	destructor;
    */
    ~AutomataComObj			( void );


public slots:


    /**
    *	@brief	change the way command strings are interpretted (characters / strings)
    */
    void			setDefaultInterpMode	( bool md )
    { parent_->setDefaultInterpMode(
                    ( md ?
                          Automata_PlugIn::IMP_STRINGS :
                          Automata_PlugIn::IMP_CHARACTERS
                          )
                    );
    }


    /**
    *	@brief	stops the execution of the program
    */
    void			stopAutomProg			( void )
    { parent_->stopAutomProg(); }


    /**
    *	@brief	strarts the execution of the program
    */
    void			runAutomProg			( void )
    { parent_->runAutomProg(); }


    /**
    *	@brief	strarts the execution of the program in step mode
    */
    void			stepAutomProg			( void )
    { parent_->stepAutomProg(); }


    /**
    *	@brief	informed when the text has changed
    */
    void			editorChanged			( void )
    { parent_->editorChanged(); }


    /**
    *	@brief	tiggered when the timeout expires on run timer
    */
    void			timeoutRun				( void )
    { parent_->performSteps( 0 ); }


    /**
    *	@brief	informed about the close of toolbar
    */
    void			tbClosing				( void )
    { parent_->noTb(); }


    /**
    *	@brief	informed about the close of tool-window
    */
    void			twClosing				( void )
    { parent_->noTw(); }




    /*  FUNCTIONS    ======================================================= */
    //
    //
    //
    //


};	/*	class AutomataComObj	*/

/*  CLASS    =============================================================== */
//
//
//
//

}	//	namespace PlugIns

#endif // __AUTOMATACOMOBJ_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
