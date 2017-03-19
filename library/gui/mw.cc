/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			mw.cc
  \date			Mar 2012
  \author		TNick

  \brief		Contains the implementation for MW class


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

#include	<dot-editor.h>


#include	<QDebug>
#include	<QFileDialog>
#include	<QFile>
#include	<QMessageBox>
#include	<QSettings>
#include	<QCloseEvent>
#include	<QTextStream>
#include	<QTreeWidget>
#include	<QDockWidget>
#include	<QProcess>
#include	<QGraphicsPixmapItem>
#include	<QPlainTextEdit>
#include	<QDesktopServices>
#include	<QColorDialog>
#include	<QTemporaryFile>
#include	<QVBoxLayout>
#include	<QComboBox>
#include	<QItemDelegate>

#include	<syntax.h>
#include	<keywords.h>
#include	<usermsg.h>
#include	<plginman.h>
#include	<impl/implinterf.h>
#include	<impl/implmanager.h>
#include	<impl/implementations/implprog.h>
#include	<impl/implementations/implgvizlib.h>


#include	<gui/aboutdlg.h>
#include	<gui/dotviewer.h>

#include	"mw.h"
#include	"ui_mw.h"

#include	"settings.h"

using namespace Gui;

MW*			MW::uniq_ = NULL;

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
MW::MW	( QWidget *parent, const QString & s_init_file) :
    QMainWindow( parent ),
    ui(),
    tv_predef (NULL),
    tv_snp (NULL),
    tx_log (NULL),
    log_dock (NULL),
    curFile (),
    b_auto_ref_ (false),
    stx_ (NULL),
    snip_file_(),
    first_run_ (false),
    impl_ (NULL),
    impl_idx_ (-1),
    save_strategy_ ()
{
    uniq_ = this;
    ui.setupUi (this);
    setAttribute (Qt::WA_DeleteOnClose, true);

    QSettings stgs;
    setWrapText (stgs.value ("mw/wraptext", false).toBool ());

    QList<int>	lnsz;
    lnsz.append (50);
    lnsz.append (50);
    ui.splitter->setSizes (lnsz);

    langList();
    createRecentFiles();
    setInternalActions();
    createSnippets();
    createLog();
    createPredef();
    prepareImg();


    /* implementations */
    ImplManager::init(); // default
    emit registerImpl(); // custom ones (a plug-in can intercept this)
    setInitImplem();


    /* prepare the Gui as expected */
    readSettings();

    /* get the file to show */
    if (s_init_file.isEmpty())
        setCurrentFile ("");
    else
        loadFile (s_init_file);

    /* show settings on first run */
    if (first_run_) {
        Settings::showSetting();
    }

    emit GuiReady();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
MW::~MW	()
{
    Q_ASSERT (uniq_ == this);
    uniq_ = NULL;

//	if (stx_ != NULL)
//	{
//		delete stx_;
//		stx_ = NULL;
//	}

//	if (impl_ != NULL)
//	{
//		delete impl_;
//		impl_ = NULL;
//	}

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void	 MW::stgsChanged			()
{

    /* snippets */
    loadSnipInTree (snip_file_);

    /* recent files */
    updateRecentFiles();

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void	 MW::setInitImplem		()
{
    QSettings	stgs;
    bool	b_ok = false;
    int crt_i = stgs.value ("mw/back-end", -1 ).toInt( &b_ok);
    if (b_ok && (crt_i != -1 ))
    {
        setImplem (crt_i);
    }
    else
    {
        crt_i = 0;
        setImplem (0);
    }
#ifndef	DE_WITH_GRAPHVIZ
    ui.actionGraphviz_library->setEnabled (false);
#endif
    ui.tb_bkends->actions().at (crt_i )->setChecked( true);

    connect( ui.tb_bkends, SIGNAL( actionTriggered(QAction*) ),
             this, SLOT(changeCrtImpl(QAction*))
             );
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void	 MW::setImplem		 (int new_impl)
{
    ImplInterf * new_one;

    ImplManager::kbCreateImpl kb = ImplManager::getImplCallback (new_impl);
    if (kb == NULL)
    {
        kb = ImplManager::getImplCallback (0);
        Q_ASSERT (kb != NULL);
    }

    new_one = kb();
    if (new_one == NULL)
    {
        kb = ImplManager::getImplCallback (0);
        Q_ASSERT (kb != NULL);
        new_one = kb();
    }

    impl_idx_ = new_impl;
    setImplem (new_one);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void	 MW::setImplem		 (ImplInterf * new_impl)
{
    QAction * crt_act;

    if (impl_ != NULL)
    {
        crt_act = impl_->action();
        if (crt_act != NULL)
        {
            crt_act->setChecked (false);
        }
        delete impl_;
    }
    impl_ = new_impl;
    if (impl_ == NULL)
    {
        ui.img_view->setScene (NULL);
    }
    else
    {
        ui.img_view->setScene (impl_->scene());
        crt_act = impl_->action();
        if (crt_act != NULL)
        {
            crt_act->setChecked (true);
        }
        impl_->perform();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void	 MW::loadSnipInTree		 (const QString & s_file)
{
    QString	s_line;
    int i; int i_max;
    int j; int j_max;
    int iter_level;
    QTreeWidgetItem	* iter_item = NULL;
    QTreeWidgetItem	* new_item = NULL;

    tv_snp->clear();

    if (s_file.isEmpty())
    {
        tx_log->appendPlainText (tr( "No snippets file defined." ));
        return;
    }

    QFile	f (s_file);
    if (f.exists() == false)
    {
        tx_log->appendPlainText(
                    QString(
                        tr( "Snippets file %1 does not exist." )
                        ).arg( s_file )
                    );
        return;
    }

    iter_level = -1;
    if (f.open( QIODevice::ReadOnly | QIODevice::Text ) == false)
    {
        tx_log->appendPlainText(
                    QString(
                        "Snippets file %1 could not be opened."
                        ).arg( s_file )
                    );
        return;
    }
    QTextStream	tx (&f);
    QStringList sl = tx.readAll().split ('\n');

    QFileInfo fi (s_file);
    iter_item = new QTreeWidgetItem (tv_snp);
    iter_item->setText (0, fi.baseName());

    i_max = sl.count();
    for ( i = 0; i < i_max; i++ )
    {
        s_line = sl.at (i);
        if (s_line.isEmpty())
            continue;
        j_max = s_line.length();
        for ( j = 0; j < j_max; j++ )
        {
            if (( s_line.at( j ) == ' ' ) || ( s_line.at( j ) == '\t' ))
                continue;
            new_item = new QTreeWidgetItem();
            /* j = the level of this item */
            if (j == iter_level)
            {
                iter_item->parent()->addChild (new_item);
            }
            else if (j < iter_level)
            {
                while ( j < iter_level )
                {
                    iter_item = iter_item->parent();
                    Q_ASSERT (iter_item != NULL);
                    iter_level--;
                }
                iter_item->parent()->addChild (new_item);
            }
            else
            {
                iter_item->addChild (new_item);
                iter_level++;
            }
            iter_item = new_item;
            iter_item->setText (0, s_line.trimmed());
            break;
        } // for ( j = 0; j < j_max; j++ )
    } // for ( i = 0; i < i_max; i++ )

    tv_snp->expandToDepth (3);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void	 MW::setSyntaxHighl	 (bool b_new_val)
{
    if (b_new_val == true)
    {
        if (stx_ != NULL)
            return;
        stx_ = new Syntax (ui.textEdit->document());
    }
    else
    {
        if (stx_ == NULL)
            return;
        delete stx_;
        stx_ = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void	 MW::changeEvent		 (QEvent * e)
{
    QMainWindow::changeEvent (e);
    switch ( e->type() ) {
    case QEvent::LanguageChange:	{
        ui.retranslateUi (this);
        break;}
    default:						{
        break;}
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void	 MW::setInternalActions	()
{

    connect( ui.actionNew, SIGNAL( triggered() ),
             this, SLOT (doNew() ));

    connect( ui.actionOpen, SIGNAL( triggered() ),
             this, SLOT (doOpen() ));

    connect( ui.actionSave, SIGNAL( triggered() ),
             this, SLOT (doSave() ));

    connect( ui.actionSave_as, SIGNAL( triggered() ),
             this, SLOT (doSave_as() ));

    connect( ui.actionExit, SIGNAL( triggered() ),
             this, SLOT (close() ));
    ui.actionExit->setShortcuts (QKeySequence::Quit);


    ui.actionUndo->setEnabled (false);
    connect( ui.actionUndo, SIGNAL( triggered() ),
             ui.textEdit, SLOT (undo() ));
    connect( ui.textEdit, SIGNAL( undoAvailable(bool) ),
             ui.actionUndo, SLOT (setEnabled(bool) ));
    ui.actionExit->setShortcuts (QKeySequence::Undo);

    ui.actionRedo->setEnabled (false);
    connect( ui.actionRedo, SIGNAL( triggered() ),
             ui.textEdit, SLOT (undo() ));
    connect( ui.textEdit, SIGNAL( redoAvailable(bool) ),
             ui.actionRedo, SLOT (setEnabled(bool) ));
    ui.actionExit->setShortcuts (QKeySequence::Redo);

    ui.actionCut->setEnabled (false);
    connect( ui.actionCut, SIGNAL( triggered() ),
             ui.textEdit, SLOT (cut() ));
    connect( ui.textEdit, SIGNAL( copyAvailable(bool) ),
             ui.actionCut, SLOT (setEnabled(bool) ));
    ui.actionExit->setShortcuts (QKeySequence::Cut);

    ui.actionCopy->setEnabled (false);
    connect( ui.actionCopy, SIGNAL( triggered() ),
             ui.textEdit, SLOT (copy() ));
    connect( ui.textEdit, SIGNAL( copyAvailable(bool) ),
             ui.actionCopy, SLOT (setEnabled(bool) ));
    ui.actionExit->setShortcuts (QKeySequence::Copy);

    connect( ui.actionPaste, SIGNAL( triggered() ),
             ui.textEdit, SLOT (paste() ));
    ui.actionExit->setShortcuts (QKeySequence::Paste);

    connect( ui.actionPreferences, SIGNAL( triggered() ),
             this, SLOT (doPreferences() ));

    connect( ui.actionSelect_all, SIGNAL( triggered() ),
             ui.textEdit, SLOT (selectAll() ));
    ui.actionSelect_all->setShortcuts (QKeySequence::SelectAll);

    connect( ui.actionAuto_refresh, SIGNAL( triggered(bool) ),
             this, SLOT (doAuto_refresh(bool) ));

    connect( ui.actionAbout, SIGNAL( triggered() ),
             this, SLOT (doAbout() ));

    connect( ui.actionAbout_Qt, SIGNAL( triggered() ),
             qApp, SLOT (aboutQt() ));

    connect( ui.actionRefreshNow, SIGNAL( triggered() ),
             this, SLOT (doRefreshNow() ));

    connect( ui.actionInsert_color, SIGNAL( triggered() ),
             this, SLOT (doInsertColor() ));

    connect( ui.actionGraphViz_site, SIGNAL( triggered() ),
             this, SLOT (doGraphViz_site() ));

    connect( ui.actionNew_instance, SIGNAL( triggered() ),
             this, SLOT (doNewInstance() ));


    connect( ui.textEdit, SIGNAL( textChanged() ),
             this, SLOT (documentWasModified() ));

    connect( ui.img_view, SIGNAL(customContextMenuRequested(QPoint)),
             this, SLOT(ctxMenu(QPoint) ) );

    connect( ui.actionWrap_text, SIGNAL(triggered(bool)),
             this, SLOT(setWrapText(bool) ) );

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::setWrapText		(bool b_enabled)
{
    ui.textEdit->setWordWrapMode( b_enabled ?
                                      QTextOption::WordWrap :
                                      QTextOption::NoWrap);
    QSettings stgs;
    stgs.setValue ("mw/wraptext", b_enabled);
    stgs.sync();
    QObject * sen = sender();
    if (qobject_cast<QAction*>(sen) == NULL) {
        ui.actionWrap_text->setChecked (b_enabled);
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::createSnippets		()
{
    QDockWidget *dock = new QDockWidget (tr( "Snippets" ), this);
    dock->setObjectName ("Snippets");
    dock->setAllowedAreas (Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tv_snp = new QTreeWidget (dock);
    dock->setWidget (tv_snp);
    addDockWidget (Qt::LeftDockWidgetArea, dock);
    ui.menuView->addAction(dock->toggleViewAction());
    tv_snp->setColumnCount(1);
    tv_snp->setHeaderHidden (true);
    connect( tv_snp, SIGNAL( itemDoubleClicked( QTreeWidgetItem*,int) ),
             this, SLOT (insertSnippet( QTreeWidgetItem*,int) ));

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::createPredef		()
{
    QDockWidget *dock = new QDockWidget (tr( "Predefined elements" ), this);
    dock->setObjectName ("Predefined");
    dock->setAllowedAreas (Qt::AllDockWidgetAreas);
    ui.menuView->addAction (dock->toggleViewAction());

    QWidget	* wg_main = new QWidget (dock);
    dock->setWidget (wg_main);
    QVBoxLayout * vbl = new QVBoxLayout (wg_main);
    wg_main->setLayout (vbl);

    QComboBox * cb_view_pred = new QComboBox (wg_main);
    vbl->addWidget (cb_view_pred);
    connect( cb_view_pred, SIGNAL( currentIndexChanged(int) ),
             this, SLOT (changePredefMode(int) ));
    cb_view_pred->addItem (tr( "Arrow heads" ));
    cb_view_pred->addItem (tr( "Box shapes" ));
    cb_view_pred->addItem (tr( "Colors" ));

    tv_predef = new QTreeWidget (wg_main);
    vbl->addWidget (tv_predef);
    connect( tv_predef, SIGNAL( itemDoubleClicked( QTreeWidgetItem*,int) ),
             this, SLOT (insertPredef( QTreeWidgetItem*,int) ));
    tv_predef->setColumnCount (2);
    tv_predef->setHeaderHidden (true);
    tv_predef->setIconSize (QSize( 64, 64 ));
    cb_view_pred->setCurrentIndex (0);
    changePredefMode (0);
    addDockWidget (Qt::LeftDockWidgetArea, dock);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::createLog		()
{
    log_dock = new QDockWidget (tr( "Log" ), this);
    log_dock->setObjectName ("Log");
    log_dock->setAllowedAreas (Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    tx_log = new QPlainTextEdit (log_dock);
    log_dock->setWidget (tx_log);
    addDockWidget (Qt::BottomDockWidgetArea, log_dock);
    ui.menuView->addAction(log_dock->toggleViewAction());

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::prepareImg			()
{
    ui.img_view->setBackgroundBrush(
                QBrush( QColor( qRgb( 0xC0, 0xC0, 0xC0 ) ),
                        Qt::DiagCrossPattern )
                );
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::doNew				()
{
    if (maybeSave())
    {
        ui.textEdit->clear();
        setCurrentFile("");
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::doOpen				()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName (this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool MW::doSave				()
{
    if (curFile.isEmpty())
    {
        return doSave_as();
    }
    else
    {
        return saveFile (curFile);
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool MW::doSave_as			()
{
    QString fileName = QFileDialog::getSaveFileName (this);
    if (fileName.isEmpty())
        return false;

    return saveFile (fileName);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::doPreferences		()
{
    Settings::showSetting();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::doRefreshNow		()
{
    genImg();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::doGraphViz_site		()
{
    QDesktopServices::openUrl (QUrl( "http://www.graphviz.org/" ));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::doNewInstance		()
{
    QSettings	stg;
    stg.sync();
    QProcess	* proc = new QProcess();
    connect( proc,SIGNAL(started()),
             this, SLOT(proc_started())
             );
    connect( proc,SIGNAL(finished(int,QProcess::ExitStatus)),
             this, SLOT(inst_finished(int,QProcess::ExitStatus))
             );
    connect( proc,SIGNAL(error(QProcess::ProcessError)),
             this, SLOT(proc_error(QProcess::ProcessError))
             );
    connect( proc,SIGNAL(readyReadStandardError()),
             this, SLOT(proc_readyError())
             );
    connect( proc,SIGNAL(readyReadStandardOutput()),
             this, SLOT(proc_readyOutput())
             );

    proc->start (QApplication::applicationFilePath());
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::doInsertColor		()
{
    QColor color = QColorDialog::getColor(
                Qt::black,
                this,
                tr( "Select color" )
                );
    if (color.isValid())
    {
        QString s_ins = QString("%1")
                .arg( color.rgb(), 0, 16 )
                .mid (2);
        s_ins = "\"#" + s_ins + "\"";

        ui.textEdit->insertPlainText (s_ins);
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::doAuto_refresh	 (bool b_on)
{
    b_auto_ref_ = b_on;
    if (b_auto_ref_)
    {
        genImg();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::doAbout				()
{
    AboutDlg	dlga;
    dlga.exec();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::documentWasModified	()
{
    setWindowModified (ui.textEdit->document()->isModified());
    if (b_auto_ref_  == true)
    {
        genImg();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::insertPredef	 (QTreeWidgetItem *item, int col)
{
    QString	s_ins = item->text (col);
    s_ins.replace ("\\\\", "\\");
    s_ins.replace ("\\n", "\n");
    s_ins.replace ("\\t", "\t");

    ui.textEdit->insertPlainText (s_ins);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::changeCrtImpl	 (QAction * new_impl)
{
    int crt_i = ui.tb_bkends->actions().indexOf (new_impl);
    if (crt_i == impl_idx_) {
        new_impl->setChecked (true);
        return;
    }
    setImplem (crt_i);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::changePredefMode (int new_idx)
{
    int i;
    QTreeWidgetItem * wit;
    /* clear the tree */
    if (tv_predef == NULL)
        return;
    tv_predef->clear();

    /* add new content based on type */
    i = 0;
    switch ( new_idx )	{
    case	0:		{ // Arrow heads
        while ( arrow_heads[i] != NULL )	{
            wit = new QTreeWidgetItem (tv_predef);
            wit->setText (0, arrow_heads[i]);
            wit->setIcon( 0, QIcon(
                              ":/resources/arrows/" +
                              QString( arrow_heads[i] ) +
                              ".gif"
                              ) );
            wit->setText( 1,
                          "a -> b [arrowhead = " +
                          QString( arrow_heads[i] ) +
                          "]" );

            i++;
        }
        break; }
    case	1:		{ // Box types
        while ( box_shapes[i] != NULL )	{
            wit = new QTreeWidgetItem (tv_predef);
            wit->setText (0, box_shapes[i]);
            wit->setIcon( 0, QIcon(
                              ":/resources/shapes/" +
                              QString( box_shapes[i] ) +
                              ".gif"
                              ) );
            wit->setText( 1,
                          "a [shape = " +
                          QString( box_shapes[i] ) +
                          "]" );
            i++;
        }
        break; }
    case	2:		{ // colors

        while ( col_names[i] != NULL )	{
            wit = new QTreeWidgetItem (tv_predef);
            wit->setText (0, col_names[i]);
            wit->setBackgroundColor (1, QColor( col_val[i] ));
            wit->setText( 1,
                          "a [color = " +
                          QString( col_names[i] ) +
                          "]" );
            i++;
        }
        break; }
    default:
        Q_ASSERT (false);
    }
    tv_predef->resizeColumnToContents (0);
    tv_predef->resizeColumnToContents (1);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::insertSnippet (QTreeWidgetItem *item, int col)
{
    QString	s_ins = item->text (col);
    s_ins.replace ("\\\\", "\\");
    s_ins.replace ("\\n", "\n");
    s_ins.replace ("\\t", "\t");

    ui.textEdit->insertPlainText (s_ins);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::closeEvent		 (QCloseEvent * event)
{
    if (maybeSave( ))
    {
        writeSettings();
        event->accept();
        ImplManager::end();

        /* terminate plug-ins */
        PlugIns::PlgInMan::end();

    }
    else
    {
        event->ignore();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString MW::defSnippFile			()
{
    QString file_o;

    file_o =
#if QT_VERSION < 0x050000
            QDesktopServices::storageLocation(
                QDesktopServices::DataLocation);
#else
            QStandardPaths::standardLocations (
                QStandardPaths::DataLocation).at(0);
#endif

    QDir	d (file_o);
    d.mkpath (".");
    file_o = d.absoluteFilePath ("Snippets.txt");

    return file_o;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QGraphicsView *	MW::graphicWidget			()
{
    return ui.img_view;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::editorTxChanged			(
        QObject *dest_obj, const char *dest_slt )
{
    Q_ASSERT (dest_obj != NULL);
    Q_ASSERT (dest_slt != NULL);

    connect(
                ui.textEdit, SIGNAL(textChanged()),
                dest_obj, dest_slt
                );

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool MW::createDefSnip	 (const QString & s_path)
{
    QFile f_dst (s_path);
    if (f_dst.open( QFile::WriteOnly | QFile::Text ))
    {
        QTextStream tst_out (&f_dst);
        QFile f_src (":/resources/dot_snippets.txt");
        if (f_src.open( QFile::ReadOnly | QFile::Text ))
        {
            QTextStream tst_in (&f_src);
            tst_out << tst_in.readAll();

            return true;
        }
    }

    return false;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::readSettings		()
{
    QSettings stgs;

    restoreGeometry(stgs.value("mw/geometry").toByteArray());
    restoreState(stgs.value("mw/windowState").toByteArray());
    ui.splitter->restoreState(
                stgs.value( "mw/splitterSizes" ).toByteArray()
                );

    ui.textEdit->setTabStopWidth(
                stgs.value( "mw/textEdit/tabsize", 40 ).toInt()
                );

    b_auto_ref_ = stgs.value("mw/autorefresh", true).toBool();
    ui.actionAuto_refresh->setChecked (b_auto_ref_);


    snip_file_ = stgs.value("snip_file", "" )
            .toString();

    if (stgs.value("syntax_h", true ).toBool())
    {
        stx_ = new Syntax (ui.textEdit->document());
    }

    bool bv = stgs.value( "mw/preview", true).toBool();
    ui.img_view->setVisible (bv);

    ui.actionPreview->setChecked (bv);
    ui.actionRefreshNow->setEnabled (bv);
    ui.actionAuto_refresh->setEnabled (bv);

    if (stgs.value( "first_run", true ).toBool())
    {
        first_run_ = true;
        stgs.setValue ("first_run", false);
        snip_file_ = defSnippFile();
        createDefSnip (snip_file_);
    }

    setEditorTextSize(
                stgs.value("mw/tx_size", 12 )
                .toInt()
                );

    save_strategy_.loadFromSettings (stgs);

    stgsChanged();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::writeSettings		()
{
    QSettings stgs;

    stgs.setValue( "mw/splitterSizes",
                   ui.splitter->saveState()
                   );
    stgs.setValue( "mw/autorefresh", b_auto_ref_);
    stgs.setValue ("snip_file", snip_file_);
    stgs.setValue ("syntax_h", (stx_ != NULL));


    stgs.setValue ("mw/geometry", saveGeometry());
    stgs.setValue ("mw/windowState", saveState());
    stgs.setValue ("mw/preview", ui.img_view->isVisible());
    stgs.setValue ("mw/tx_size", ui.textEdit->font().pointSize());


    stgs.setValue( "mw/textEdit/tabsize",
                   ui.textEdit->tabStopWidth()
                   );

    /* save current back-end */
    if (impl_ == NULL)
    {
        stgs.setValue ("mw/back-end", -1);
    }
    else
    {
        stgs.setValue( "mw/back-end",
                       ui.tb_bkends->actions().indexOf( impl_->action() )
                       );
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool MW::maybeSave			()
{
    if (ui.textEdit->document()->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(
                    this, DOTEDITOR_PROJECT_NAME,
                    tr("The document has been modified.\n"
                       "Do you want to save your changes?"),
                    QMessageBox::Save | QMessageBox::Discard |
                    QMessageBox::Cancel
                    );
        if (ret == QMessageBox::Save)
            return doSave();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::loadFile	 (const QString & fileName)
{
    QFile file(fileName);
    if (!file.open( QFile::ReadOnly | QFile::Text ))
    {
        QString s_e = tr( "Cannot read file %1:\n%2." )
                .arg(fileName)
                .arg(file.errorString() );
        QMessageBox::warning(this,
                             DOTEDITOR_PROJECT_NAME,
                             s_e
                             );
        tx_log->appendPlainText (s_e);
        return;
    }

    QTextStream in (&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor (Qt::WaitCursor);
#endif
    ui.textEdit->setPlainText (in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile (fileName);
    statusBar()->showMessage (tr("File loaded"), 2000);
    tx_log->appendPlainText(
                QString (tr( "%1 succesfully loaded." ) ).arg( fileName ));

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool MW::saveFile	 (const QString & fileName)
{
    QFile file(fileName);
    if (!file.open (QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning( this,
                              DOTEDITOR_PROJECT_NAME,
                              tr( "Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString() )
                              );
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << ui.textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile (fileName);
    statusBar()->showMessage (tr ("File saved"), 2000);
    tx_log->appendPlainText(
                QString (tr( "%1 succesfully saved." ) ).arg( fileName ));

    for (;;) {
        if (save_strategy_.strategy_ == SaveStrategy::Simple) {
            break;
        }
        Q_ASSERT(save_strategy_.strategy_ == SaveStrategy::Entangled);

        /// @TODO

        break;
    }

    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::setCurrentFile (const QString & fileName)
{
    curFile = fileName;
    ui.textEdit->document()->setModified (false);

    QString shownName = curFile;
    if (curFile.isEmpty())
    {
        tx_log->appendPlainText (tr( "New file loaded in editor" ));
        shownName = tr ("untitled");
        setWindowTitle (shownName + "[*] - " DOTEDITOR_PROJECT_NAME);
    }
    else
    {
        updateRecentFiles (curFile);
        QFileInfo fi (fileName);
        setWindowTitle (fi.fileName() + "[*] (" + fi.path() + ") - " DOTEDITOR_PROJECT_NAME);
    }
    setWindowFilePath (shownName);
    setWindowModified (false);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString MW::strippedName				(
        const QString & fullFileName )
{
    return QFileInfo(fullFileName).fileName();
}
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
bool MW::genImg		 (bool b_force)
{
    if (ui.img_view->isVisible() == false)
    {
        if (b_force == false)
            return false;
    }

    return impl_->perform ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::setSnipFile		 (const QString & s_new_val)
{
    snip_file_ = s_new_val;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::ctxMenu			 (QPoint pt)
{

    impl_->ctxDisplay (pt, ui.img_view->mapToGlobal( pt ));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::setEditorTextSize (int new_size)
{
    QFont	fnt = ui.textEdit->font();
    fnt.setPointSize (new_size);
    ui.textEdit->setFont (fnt);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::setEditorFont	 (const QFont & fnt)
{
    QFont	fnt_new (fnt);
    QFont	fnt_now = ui.textEdit->font();
    fnt_new.setPointSize (fnt_now.pointSize());
    ui.textEdit->setFont (fnt_new);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::langChange		 (QAction * p_crt_act)
{
    QDir	d(
            #if QT_VERSION < 0x050000
                        QDesktopServices::storageLocation(
                            QDesktopServices::DataLocation)
            #else
                        QStandardPaths::standardLocations (
                            QStandardPaths::DataLocation).at(0)
            #endif
                );



    if (d.exists() == false) return;
    if (d.exists( DATA_FLD_TRANSL ) == false) return;
    d.cd (DATA_FLD_TRANSL);

    if (p_crt_act->text() != "English")
    {
        QString s_loc = LANG_FILE (p_crt_act->text());
        if (d.exists( s_loc ) == false) return;
    }
    QSettings().setValue (STG_LANGUAGE, p_crt_act->text());


    QMessageBox::information(
                this, DOTEDITOR_PROJECT_NAME,
                tr(
                    "The language change was acknowledged\n"
                    "but it will take effect only the next\n"
                    "time the application will be started."
                    ) );
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::langList			()
{
    QDir	d(
            #if QT_VERSION < 0x050000
                QDesktopServices::storageLocation(
                    QDesktopServices::DataLocation)
            #else
                QStandardPaths::standardLocations (
                    QStandardPaths::DataLocation).at(0)
            #endif
                );

    QActionGroup *	langGroup;
    QAction *		ac;
    QString			s_lang =
            QSettings().value(
                STG_LANGUAGE,
                QLocale::system().name()
                ).toString();
    QString			l_mark;

    langGroup = new QActionGroup (ui.menuLanguage);
    langGroup->setExclusive (true);

    ac = langGroup->addAction ("English");
    ac->setCheckable (true);
    ac->setChecked (true);

    for ( ;; )
    {
        if (d.exists() == false) break;
        if (d.exists( DATA_FLD_TRANSL ) == false) break;
        d.cd (DATA_FLD_TRANSL);
        QStringList list = d.entryList  (QStringList( "*" LANG_FILE_SUFFIX ));
        int i; int i_max; int j; int k;
        i_max = list.count();

        for ( i = 0; i < i_max; i++ )
        {
            j = list.at (i ).lastIndexOf( '.');
            Q_ASSERT (j != -1);
            // dot-editor_lang.qm
            k = 11;
            Q_ASSERT (sizeof( LANG_FILE_PREFIX ) - 1 == k);
            Q_ASSERT (j > k);
            l_mark = list.at (i ).mid( k, j - k);

            ac = langGroup->addAction (l_mark);
            ac->setCheckable (true);
            if (l_mark == s_lang)
                ac->setChecked (true);
            else
                ac->setChecked (false);

        }
        break;
    }
    connect( langGroup, SIGNAL( triggered( QAction* ) ),
             this, SLOT (langChange( QAction* ) ));

    ui.menuLanguage->addActions (langGroup->actions());
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::openRecentFile			()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action != NULL)
    {
        if (maybeSave())
        {
            loadFile (action->text());
        }
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::createRecentFiles		()
{
    updateRecentFiles ("");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void MW::updateRecentFiles	 (const QString & new_rf)
{
    QStringList		sl;
    QSettings		stg;
    QAction *		ac;
    QList<QAction*>	ac_lst = ui.menuOpen_recent_files->actions();
    int				ac_c = ac_lst.count();

    sl = stg.value( RECENT_FILES_LIST ).toStringList();
    if (new_rf.isEmpty() == false)
    {
        sl.prepend (new_rf);
        sl.removeDuplicates();
    }
    bool	b_ok;
    int rc_f = stg.value (RECENT_FILES_COUNT, 10 ).toInt( &b_ok);
    if (b_ok == false)
        rc_f = 10;
    else if (rc_f < 0)
        rc_f = 10;

    while ( sl.count() > rc_f )
    {
        sl.removeLast();
    }
    rc_f = sl.count();

    if (rc_f > ac_c)
    {
        do	{
            ac = ui.menuOpen_recent_files->addAction ("");
            connect( ac, SIGNAL( triggered() ),
                     this, SLOT( openRecentFile() )
                     );
            ac_lst.append (ac);
            ac_c++;
        } while  (rc_f > ac_c);
    }
    else if (rc_f < ac_c)
    {
        do	{
            ac_c--;
            ac_lst.at( ac_c )->deleteLater();
        } while  (rc_f < ac_c);
    }
    /* same number of actions as recent files count */
    for ( int i = 0; i < rc_f; i++ )
    {
        ac_lst.at (i )->setText( sl.at( i ));
        ac_lst.at (i )->setVisible( true);
    }

    stg.setValue (RECENT_FILES_LIST, sl);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			MW::showLogMessage		 (UserMsg * p_msg)
{
    QTextCursor cursor = tx_log->textCursor();
    cursor.atEnd();
    cursor.insertHtml (p_msg->toHtml());

    tx_log->ensureCursorVisible();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void			MW::showLogMessage		 (void * pmsg)
{
    UserMsg::MsgCacheList * p_msg = (UserMsg::MsgCacheList*)pmsg;
    QTextCursor cursor = tx_log->textCursor();
    cursor.atEnd();
    cursor.insertHtml (UserMsg::cacheListToHtml( * p_msg ));

    tx_log->ensureCursorVisible();
}
/* ========================================================================= */



/* Intrface with the GUI for UserMsg */

/* ------------------------------------------------------------------------- */
bool		GUI_isCmdLogVisible		()
{
    return MW::unique()->logVisibility();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void		GUI_showCmdLog			()
{
    MW::unique()->setLogVisibility (true);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void		GUI_showCmdLogMessage (UserMsg::MsgCacheList * p_msg)
{
    MW::unique()->showLogMessage (p_msg);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void		GUI_showCmdLogMessage (UserMsg * p_msg)
{
    MW::unique()->showLogMessage (p_msg);
}
/* ========================================================================= */


/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
