/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			mw.h
  \date			Mar 2012
  \author		TNick

  \brief		Contains the definition for MW class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __MW_INC__
#define __MW_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include    <QMainWindow>
#include	<QProcess>
#include	<QDockWidget>

#include	"ui_mw.h"
#include	"savestrategy.h"

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

class	QFileDialog;
class	QFile;
class	QMessageBox;
class	QSettings;
class	QCloseEvent;
class	QTextStream;
class	QTreeWidget;
class	QTreeWidgetItem;
class	QDockWidget;
class	QPlainTextEdit;
class	QSyntaxHighlighter;

class	UserMsg;
class	ImplInterf;

namespace Ui {
class MW;
}

namespace	Gui		{

/**
*	@brief	Form that represents main panel for application
*/
class MW : public QMainWindow		{
    Q_OBJECT
    //
    //
    //
    //
    /*  DEFINITIONS    ----------------------------------------------------- */

#define	STG_LANGUAGE		"ui-language"
#define	DATA_FLD_TRANSL		"translations"
#define	LANG_FILE_PREFIX	"dot-editor_"
#define	LANG_FILE_SUFFIX	".qm"
#define	LANG_FILE(loc)		(LANG_FILE_PREFIX + loc + LANG_FILE_SUFFIX)
#define	RECENT_FILES_COUNT	"recent_files_count"
#define	RECENT_FILES_LIST	"recent_files_list"

    /*  DEFINITIONS    ===================================================== */
    //
    //
    //
    //
    /*  DATA    ------------------------------------------------------------ */

private:

    /**
     *	@name		GUI components
     */
    ///@{
    Ui::MW ui; /**< GUI components embedded here  */
    QTreeWidget * tv_predef; /**< treeview for predefined elements */
    QTreeWidget * tv_snp; /**< treeview for snippets */
    QPlainTextEdit * tx_log; /**< log panel */
    QDockWidget * log_dock; /**< log dock panel */
    ///@}


    QString curFile; /**< current file ( empty = new file ) */
    bool b_auto_ref_; /**< autorefresh status; if true,
        image is regenerated each time the user types something */
    QSyntaxHighlighter * stx_; /**< syntax highliting is enabled or not */
    QString snip_file_; /**< the file used with snippets */
    bool first_run_; /**< tell if this is the first time we run this */
    ImplInterf * impl_; /**< current implementation */
    int impl_idx_; /**< index of current implementation */
    SaveStrategy save_strategy_; /**< How to save image files. */

    static MW* uniq_; /**< the one and only instance */

    /*  DATA    ============================================================ */
    //
    //
    //
    //
    /*  FUNCTIONS    ------------------------------------------------------- */

public:

    //! Default constructor.
    explicit MW (
            QWidget *parent=0,
            const QString & s_init_file=QString ());

    //! Destructor.
    ~MW		();

    //! The one and only instance
    static MW *
    unique () {
        return uniq_;
    }

    //! Access to save strategy.
    SaveStrategy &
    saveStrategy () {
        return save_strategy_;
    }

    //! Access to save strategy.
    const SaveStrategy &
    saveStrategy () const {
        return save_strategy_;
    }

    //! get the size of the tab in pixels
    inline int			tabWidth				()
    { return ui.textEdit->tabStopWidth(); }


    /**
    *	@brief	get the size of the tab in pixels
    */
    inline QString		snipFile				()
    { return snip_file_; }


    /**
    *	@brief	tell if syntax highliting is on or off
    */
    inline bool			syntaxHighl				()
    { return  (stx_ != NULL); }


    /**
    *	@brief	create default snippets file on file system
    */
    bool				createDefSnip			 (const QString & s_path);


    /**
    *	@brief	create default snippets file on file system
    */
    QString				defSnippFile			 ();


    /**
    *	@brief	change current implementation
    */
    void				setImplem				(
            ImplInterf *				new_impl
            );


    /**
    *	@brief	change current implementation
    */
    void				setImplem				(
            int							new_impl
            );


    /**
    *	@brief	get current implementation
    */
    ImplInterf *		implem			()
    { return impl_; }



    /**
    *	@brief	the widget used to display nifty things
    */
    QGraphicsView *		graphicWidget			 ();



    /**
    *	@brief	the size of the text in editor
    */
    QString				editorText				()
    { return ui.textEdit->toPlainText(); }


    /**
    *	@brief	the size of the text in editor
    */
    int					editorTextSize			()
    { return ui.textEdit->font().pointSize(); }


    /**
    *	@brief	the name of the font
    */
    QFont				editorFont				()
    { return ui.textEdit->font(); }


    /**
    *	@brief	connect to the text-changed signal
    */
    void				editorTxChanged			 (
            QObject *				dest_obj,
            const char *			dest_slt
            );


    /**
    *	@brief	tell if log is visible or not
    */
    inline bool			logVisibility			()
    { return log_dock->isVisible( ); }


    /**
    *	@brief	set if log is visible or not
    */
    inline void			setLogVisibility	 (bool b_viz)
    { return log_dock->setVisible (b_viz); }


    /**
    *	@brief	log a message
    */
    void				showLogMessage			 (UserMsg * p_msg);


    /**
    *	@brief	log a message
    */
    void				showLogMessage			 (void * p_msg);


    /**
    *	@brief	Predefined action for Progam implementation
    */
    inline QAction *	predefAct_Prog			()
    { return ui.actionExternal_program; }


    /**
    *	@brief	Predefined action for GraphViz implementation
    */
    inline QAction *	predefAct_GViz			()
    { return ui.actionGraphviz_library; }


    /**
    *	@brief	add an action to View menu
    */
    inline void			addMenuViewAction	 (QAction * new_val)
    { ui.menuView->addAction (new_val); }

signals:



    /**
    *	@brief	emmited when custom implementations should be added
    */
    void				registerImpl			 ();


    /**
    *	@brief	emmited when the gui is fully loaded and ready to start
    */
    void				GuiReady				 ();



public slots:


    /**
    *	@brief	the settings have changed so the cache is invalid
    */
    void				stgsChanged				 ();



    /**
    *	@brief	the settings have changed so the cache is invalid
    */
    void				setTabWidth			 (int new_val)
    { ui.textEdit->setTabStopWidth (new_val); }



    /**
    *	@brief	the settings have changed so the cache is invalid
    */
    void				setSnipFile				(
            const QString & s_new_value
            );


    /**
    *	@brief	load a file in the tree of snippets
    */
    void				loadSnipInTree			(
            const QString & s_file
            );


    /**
    *	@brief	turn syntax highliting on or off
    */
    void				setSyntaxHighl			(
            bool			b_new_val
            );


    /**
    *	@brief	change the size of the text in editor
    */
    void				setEditorTextSize	 (int new_size);


    /**
    *	@brief	change the font in editor
    */
    void				setEditorFont		 (const QFont &fnt);


    /**
    *	@brief	used by recent files actions
    */
    void				openRecentFile		 ();


    /**
    *	@brief	toogles word wrap on or off
    */
    void                setWrapText          (bool b_enabled);


private:


    /**
    *	@brief	connects actions from inside MW to their slots
    */
    void				setInternalActions		 ();


    /**
    *	@brief	create and set-up the dock for snippets
    */
    void				createSnippets			 ();

    /**
    *	@brief	create and set-up the dock for predefined elements
    */
    void				createPredef			 ();

    /**
    *	@brief	create and set-up the dock for log
    */
    void				createLog				 ();

    /**
    *	@brief	prepare image related components at start-up
    */
    void				prepareImg				 ();

    /**
    *	@brief	prepare the recent files system at start-up
    */
    void				createRecentFiles		 ();


    /**
    *	@brief	used to get settings for main window
    */
    void				readSettings			 ();


    /**
    *	@brief	save settings for main window
    */
    void				writeSettings			 ();


    /**
    *	@brief	if the document has changes it attempts to save it
    */
    bool				maybeSave				 ();


    /**
    *	@brief	reads the file and loads its content in editor
    */
    void				loadFile				 (const QString & fileName);


    /**
    *	@brief	saves the content of the editor to designated file
    */
    bool				saveFile				 (const QString & fileName);


    /**
    *	@brief	changing the file
    */
    void				setCurrentFile			 (const QString & fileName);


    /**
    *	@brief	gets the name without path
    */
    QString				strippedName			 (const QString & fullFileName);


    /**
    *	@brief	get a list of available languages and select current one
    */
    void				langList				 ();


    /**
    *	@brief	updates the list of recent files;
    */
    void				updateRecentFiles		(
            const QString & new_rf = QString()
            );


    /**
    *	@brief	change current implementation
    */
    void				setInitImplem			 ();



private slots:


    /**
    *	@brief	Creates a new file
    */
    void		doNew			 ();


    /**
    *	@brief	Open an existing file
    */
    void		doOpen			 ();


    /**
    *	@brief	Saves current file
    */
    bool		doSave			 ();


    /**
    *	@brief	Saves current file on a different location
    */
    bool		doSave_as		 ();

    /**
    *	@brief	Show preferences dialog
    */
    void		doPreferences	 ();


    /**
    *	@brief	refresh the view
    */
    void		doRefreshNow	 ();

    /**
    *	@brief	create a new instance of this application
    */
    void		doNewInstance	 ();

    /**
    *	@brief	let the user select a color then insert it at current position
    */
    void		doInsertColor	 ();

    /**
    *	@brief	opens GraphViz site in default browser
    */
    void		doGraphViz_site	 ();

    /**
    *	@brief	Switch auto-refresh on or off
    */
    void		doAuto_refresh	 (bool b_on);


    /**
    *	@brief	Show About dialog
    */
    void		doAbout			 ();


    /**
    *	@brief	informed about changes in document
    */
    void		documentWasModified		 ();


    /**
    *	@brief	inserts the content of the item at current cursor position
    */
    void		insertSnippet			 (QTreeWidgetItem* item , int col);


    /**
    *	@brief	inserts the content of the item at current cursor position
    */
    void		insertPredef			 (QTreeWidgetItem* item , int col);


    /**
    *	@brief	changes what is displayed in predefined tree
    */
    void		changePredefMode		 (int new_idx);


    /**
    *	@brief	changes what is displayed in predefined tree
    */
    void		changeCrtImpl			 (QAction * new_impl);


    /**
    *	@brief	creates an image from the text in editor
    */
    bool		genImg					 (bool b_force = false);


    /**
    *	@brief	contextual menu
    */
    void		ctxMenu					 (QPoint pt);


    /**
    *	@brief	informed about a language change
    */
    void		langChange				 (QAction * p_crt_act);





protected:


    /**
    *	@brief	Implemented to allow trapping the language change event
    */
    void		changeEvent				 (QEvent *e);


    /**
    *	@brief	Implemented to allow saing the changes before exit
    */
    void		closeEvent				 (QCloseEvent *event);





    /*  FUNCTIONS    ======================================================= */
    //
    //
    //
    //

};	/*	class MW	*/



/*  CLASS    =============================================================== */
//
//
//
//

}	//	namespace	Gui

#endif // __MW_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
