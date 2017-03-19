/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			settings.cpp
  \date			Mar 2012
  \author		TNick

  \brief		Contains the implementation for Settings class


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
#include	<QSettings>
#include	<QPushButton>
#include	<QFileDialog>

#include	<plginman.h>
#include	<usermsg.h>
#include	<impl/implementations/implprog.h>
#include	<gui/mw.h>

#include	"settings.h"
#include	"ui_settings.h"
#include	"savestrategy.h"

using namespace Gui;
using namespace PlugIns;

Settings *	Settings::uniq_ = NULL;


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
Settings::Settings( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::Settings )
{
    QSettings stg;
    int		i;
    bool	b;

    Q_ASSERT (uniq_ == NULL);
    ui->setupUi(this);
    uniq_ = this;
    setAttribute (Qt::WA_DeleteOnClose, true);

    QPushButton* a_button = ui->buttonBox->button(
                QDialogButtonBox::Apply );
    connect( ui->buttonBox, SIGNAL( accepted() ),
             this, SLOT( saveStgToMain() )
             );
    connect( a_button, SIGNAL( clicked() ),
             this, SLOT( saveStgToMain() )
             );

    a_button = ui->buttonBox->button(
                QDialogButtonBox::RestoreDefaults );
    connect( a_button, SIGNAL( clicked() ),
             this, SLOT( resetToDef() )
             );

    a_button = ui->buttonBox->button(
                QDialogButtonBox::Reset );
    connect( a_button, SIGNAL( clicked() ),
             this, SLOT( resetToMain() )
             );

    connect( ui->b_prog, SIGNAL( clicked() ),
             this, SLOT( selProgram() )
             );
    connect( ui->b_snip_db, SIGNAL( clicked() ),
             this, SLOT( selSnipDbf() )
             );
    connect( ui->le_snip_file, SIGNAL( editingFinished() ),
             this, SLOT( changeSnipDbf() )
             );
    connect( ui->tx_snipp, SIGNAL( textChanged() ),
             this, SLOT( txSnipChanged() )
             );

    connect( ui->plug_in_tree, SIGNAL( doubleClicked(QModelIndex) ),
             this, SLOT( dbClickPlugIns(QModelIndex) )
             );


    restoreGeometry(stg.value("stg/geometry").toByteArray());
    resetToMain();
    changeSnipDbf();

    i = stg.value ("stg/activeTab", 0 ).toInt( &b);
    if (b == false)
        i = 0;
    ui->tabWidget->setCurrentIndex (i);

    show();
    b_snipp_ed_ = false;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Settings::~Settings()
{
    Q_ASSERT (uniq_ == this);
    QSettings stg;
    stg.setValue( "stg/geometry", saveGeometry());
    stg.setValue ("stg/activeTab", ui->tabWidget->currentIndex());
    delete ui;
    uniq_ = NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::changeEvent		 (QEvent * e)
{
    QDialog::changeEvent (e);
    switch ( e->type() ) {
    case QEvent::LanguageChange:	{
        ui->retranslateUi (this);
        break;}
    default:						{
        break;}
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::closeEvent		 (QCloseEvent * event)
{
    event->accept();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::selProgram			()
{

    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Select the program"),
                ui->le_prog->text()
                );
    if (fileName.isEmpty() == false)
    {
        ui->le_prog->setText (fileName);
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::selSnipDbf			()
{

    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Select the program"),
                ui->le_snip_file->text()
                );
    if (fileName.isEmpty() == false)
    {
        ui->le_snip_file->setText (fileName);
        changeSnipDbf();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::txSnipChanged			()
{
    b_snipp_ed_ = true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::dbClickPlugIns (const QModelIndex & mi)
{
    QTreeWidgetItem *			pl_tr_it;
    QBrush						br_bk (Qt::lightGray);
    Qt::GlobalColor				gc_tx;
    bool						bld;

    //pl_tr_it = ui->plug_in_tree->itemFromIndex (mi);
    Q_UNUSED (mi);
    pl_tr_it = ui->plug_in_tree->currentItem();
    if (pl_tr_it == NULL)
        return;

    QFont	f = pl_tr_it->font (0);
    if (pl_tr_it->background(0) == br_bk)
    { /* is loadable */
        br_bk.setColor (Qt::white);
        gc_tx = Qt::black;
        bld = false;
    }
    else
    { /* is not loadable */
        gc_tx = Qt::blue;
        bld = true;
    }

    pl_tr_it->setBackground (0, br_bk);
    pl_tr_it->setBackground (1, br_bk);
    pl_tr_it->setTextColor (0, QColor( gc_tx ));
    pl_tr_it->setTextColor (1, QColor( gc_tx ));
    f.setBold (bld);
    pl_tr_it->setFont (0, f);
    pl_tr_it->setFont (1, f);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::getPlugInList			()
{
    QString						s_plg_path;
    QList<PlgInMan::PlgInInfo>	plg_l = PlgInMan::listPlugIns (s_plg_path);
    PlgInMan::PlgInInfo			pl_it;
    QStringList					sl;
    QTreeWidgetItem *			pl_tr_it;
    QBrush						br_bk (Qt::lightGray);
    QFont						f;

    ui->plug_in_tree->clear();
    ui->plug_in_tree->setHeaderHidden (false);

    QFlags<Qt::ItemFlag> flg =
            Qt::ItemIsUserCheckable |
            Qt::ItemIsSelectable |
            Qt::ItemIsEnabled;
    QStringList	sl_n;

    foreach( pl_it, plg_l )		{
//		if (sl_n.contains( pl_it.s_name ))
//			continue;
        sl_n.append (pl_it.s_name);
        sl.clear();
        sl.append (pl_it.s_name);
        sl.append (pl_it.s_full_path);

        pl_tr_it = new QTreeWidgetItem(
                    ui->plug_in_tree, sl
                    );
        pl_tr_it->setFlags (flg);
        pl_tr_it->setData(
                    0,
                    Qt::UserRole,
                    QVariant::fromValue( (void*) pl_it.p_interf )
                    );

        if (pl_it.b_loaded)
        {
            pl_tr_it->setCheckState (0, Qt::Checked);
        }
        else
        {
            pl_tr_it->setCheckState (0, Qt::Unchecked);
        }

        if (pl_it.b_start_up)
        {
            pl_tr_it->setBackground (0, br_bk);
            pl_tr_it->setBackground (1, br_bk);
            pl_tr_it->setTextColor (0, Qt::blue);
            pl_tr_it->setTextColor (1, Qt::blue);
            f = pl_tr_it->font (0);
            f.setBold (true);
            pl_tr_it->setFont (0, f);
            pl_tr_it->setFont (1, f);
        }
    }
    ui->plug_in_tree->resizeColumnToContents (0);
    ui->plug_in_tree->resizeColumnToContents (1);

    ui->le_plugin_loc->setText (s_plg_path);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::saveSaveStrategy		(SaveStrategy & data)
{
    if (ui->simpleSaveRadioButton->isChecked ()) {
        data.strategy_ = SaveStrategy::Simple;
    } else {
        Q_ASSERT(ui->entangledSaveRadioButton->isChecked ());
        data.strategy_ = SaveStrategy::Entangled;
    }

    if (ui->imagePathInTextCheckBox->isChecked ()) {
        if (ui->absoluteImagePathRadioButton->isChecked ()) {
            data.attach_ = SaveStrategy::AttachAbsolutePath;
        } else {
            Q_ASSERT(ui->relativeImagePathRadioButton->isChecked ());
            data.attach_ = SaveStrategy::AttachRelativePath;
        }
    } else {
        data.attach_ = SaveStrategy::AttachNone;
    }

    if (ui->imageHasSamePathRadioButton->isChecked ()) {
        data.entangle_ = SaveStrategy::SamePathAndName;
    } else if (ui->imageAskOnFirstSaveRadioButton->isChecked ()) {
        data.entangle_ = SaveStrategy::AskOnFirstSave;
    } else {
        Q_ASSERT(ui->saveImageAfterRequestRadioButton->isChecked ());
        data.entangle_ = SaveStrategy::WaitForSaveAs;
    }

    data.saveToSettings (QSettings());
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::loadSaveStrategy		(const SaveStrategy & data)
{
    switch (data.entangle_) {
    case SaveStrategy::SamePathAndName: {
        ui->imageHasSamePathRadioButton->setChecked (true);
        break; }
    case SaveStrategy::AskOnFirstSave: {
        ui->imageAskOnFirstSaveRadioButton->setChecked (true);
        break; }
    case SaveStrategy::WaitForSaveAs: {
        ui->saveImageAfterRequestRadioButton->setChecked (true);
        break; }
    default:
        Q_ASSERT(false);
    }

    switch (data.attach_) {
    case SaveStrategy::AttachAbsolutePath: {
        ui->imagePathInTextCheckBox->setChecked (true);
        ui->absoluteImagePathRadioButton->setChecked (true);
        break; }
    case SaveStrategy::AttachRelativePath: {
        ui->imagePathInTextCheckBox->setChecked (true);
        ui->relativeImagePathRadioButton->setChecked (true);
        break; }
    case SaveStrategy::AttachNone: {
        ui->imagePathInTextCheckBox->setChecked (false);
        break; }
    default:
        Q_ASSERT(false);
    }

    switch (data.entangle_) {
    case SaveStrategy::Entangled: {
        ui->entangledSaveRadioButton->setChecked (true);
        break; }
    case SaveStrategy::Simple: {
        ui->simpleSaveRadioButton->setChecked (true);
        break; }
    default:
        Q_ASSERT(false);
    }

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::savePlugInList		()
{
    QTreeWidgetItem *			pl_tr_it;
    QList<PlgInMan::PlgInInfo>	plg_l;
    PlgInMan::PlgInInfo			pl_it;
    UserMsg						um;
    QBrush						br_bk (Qt::lightGray);

    int it_cnt = ui->plug_in_tree->topLevelItemCount();
    for ( int i = 0; i < it_cnt; i++ )
    {
        pl_tr_it = ui->plug_in_tree->topLevelItem (i);
        pl_it.s_name = pl_tr_it->text (0);
        pl_it.s_full_path = pl_tr_it->text (1);
        pl_it.b_loaded = pl_tr_it->checkState(0);
        pl_it.b_start_up =  (pl_tr_it->background(0) == br_bk);
        pl_it.p_interf = (PlugIn *)
                pl_tr_it->data( 0, Qt::UserRole )
                .value<void*>();
        plg_l.append (pl_it);
    }
    if ( ! OUT_FULL_SUCCESS(
             PlgInMan::syncListPlugIns( plg_l, um ) ) )
    {
        um.show();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::changeSnipDbf			()
{
    ui->tx_snipp->clear();

    QFile	f (ui->le_snip_file->text());
    if (f.exists())
    {
        if (f.open( QIODevice::ReadOnly | QIODevice::Text ))
        {
            QTextStream	tx (&f);
            ui->tx_snipp->setPlainText (tx.readAll());
            b_snipp_ed_ = false;
        }
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::resetToDef			()
{
    MW * mw = MW::unique();
    ui->le_prog->setText ("dot");
    ui->te_args->setText ("-Tpng -o $OUT $IN" );
    ui->le_inp_file->setText ("$TEMP/dot_XXXXXXXX.dot");
    ui->le_out_file->setText ("$TEMP/dot_XXXXXXXX.png");
    ui->sp_tab_size->setValue (40);
    ui->tx_snipp->setTabStopWidth (40);
    ui->ck_syntax_h->setChecked (true);
    //ui->cb_font->setFont (QFont());
    ui->sp_text_size->setValue (10);

    QString s_ds = mw->defSnippFile();
    if (QFile( s_ds ).exists() == false)
    {
        mw->createDefSnip (s_ds);
    }
    ui->le_snip_file->setText (s_ds);

    changeSnipDbf();

    ui->sp_recent_flen->setValue (10);

    ui->simpleSaveRadioButton->setChecked (true);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::saveStgToMain			()
{
    QSettings	stg;
    MW * mw = MW::unique();

    QFile	f (ui->le_snip_file->text());
    if (f.open( QIODevice::WriteOnly | QIODevice::Text ))
    {
        QTextStream	tx (&f);
        tx << ui->tx_snipp->toPlainText();
    }

    ImplProg::Stg s_p;
    s_p.s_arg_ = ui->te_args->toPlainText();
    s_p.s_process_ = ui->le_prog->text().trimmed();
    s_p.tmp_src_ = ui->le_inp_file->text();
    s_p.tmp_dest_ = ui->le_out_file->text();
    ImplProg::setSettings (s_p);


    mw->setTabWidth (ui->sp_tab_size->value());
    mw->setSyntaxHighl (ui->ck_syntax_h->isChecked());

    mw->setEditorTextSize (ui->sp_text_size->value());
    //mw->setFont (ui->cb_font->font());

    mw->setSnipFile (ui->le_snip_file->text());

    stg.setValue (RECENT_FILES_COUNT, ui->sp_recent_flen->value());

    savePlugInList();
    saveSaveStrategy (mw->saveStrategy ());

    mw->stgsChanged();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::resetToMain			()
{
    QSettings	stg;
    MW * mw = MW::unique();

    /* ImplProg specific - build in*/
    ImplProg::Stg s_p = ImplProg::settings();
    ui->le_prog->setText (s_p.s_process_);
    ui->te_args->setText (s_p.s_arg_);
    ui->le_inp_file->setText (s_p.tmp_src_);
    ui->le_out_file->setText (s_p.tmp_dest_);


    ui->sp_tab_size->setValue (mw->tabWidth());
    ui->ck_syntax_h->setChecked (mw->syntaxHighl());

    //ui->cb_font->setFont (mw->editorFont());
    ui->sp_text_size->setValue (mw->editorTextSize());

    ui->le_snip_file->setText (mw->snipFile());
    ui->tx_snipp->setTabStopWidth (mw->tabWidth());

    bool b_ok;
    ui->sp_recent_flen->setValue(
                stg.value( RECENT_FILES_COUNT, 10 ).toInt( &b_ok )
                );
    if (b_ok == false)
    {
        ui->sp_recent_flen->setValue (10);
    }

    getPlugInList();
    loadSaveStrategy (mw->saveStrategy ());
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Settings::showSetting			()
{
    if (uniq_ == NULL)
    {
        new Settings (MW::unique());
    }
    else
    {
        uniq_->setVisible (true);
        uniq_->raise();
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
