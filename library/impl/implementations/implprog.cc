/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			implprog.cc
  \date			May 2012
  \author		TNick

  \brief		Contains the implementation of ImplProg class


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

#include	<QFile>
#include	<QTemporaryFile>
#include	<QSettings>
#include	<QMenu>
#include	<QAction>
#include	<QGraphicsPixmapItem>
#include	<QFileDialog>
#include	<QDesktopServices>
#include	<QDebug>

#include	<usermsg.h>
#include	<gui/guictrl.h>
#include	<gui/mw.h>


#include	"implprog.h"


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


ImplProg *		ImplProg::uniq_ = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
ImplProg::ImplProg	()
    : ImplInterf()
{
    Q_ASSERT (uniq_ == NULL);
    uniq_ = this;
    dot_process = NULL;
    itm_img = NULL;

    s_.load();
    applySettings();

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ImplProg::~ImplProg	()
{

    if (QFile::exists( cache_tmp_src_ ))
    {
        QFile::remove (cache_tmp_src_);
    }
    if (QFile::exists( cache_tmp_dest_))
    {
        QFile::remove (cache_tmp_dest_);
    }

    s_.save();

    Q_ASSERT (uniq_ == this);
    uniq_ = NULL;

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::stopDotProcess	()
{
    if (dot_process != NULL)
    {
        dot_process->terminate();
        dot_process->waitForFinished();
        delete dot_process;
        dot_process = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::startDotProcess	()
{

    dot_process = new QProcess (qApp);
    connect( dot_process,SIGNAL(finished(int,QProcess::ExitStatus)),
             this, SLOT(proc_finished(int,QProcess::ExitStatus))
             );
    connect( dot_process,SIGNAL(started()),
             this, SLOT(proc_started())
             );
    connect( dot_process,SIGNAL(error(QProcess::ProcessError)),
             this, SLOT(proc_error(QProcess::ProcessError))
             );
    connect( dot_process,SIGNAL(readyReadStandardError()),
             this, SLOT(proc_readyError())
             );
    connect( dot_process,SIGNAL(readyReadStandardOutput()),
             this, SLOT(proc_readyOutput())
             );

    dot_process->start ("\"" + s_.s_process_ + "\" " + cache_s_arg_);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::inst_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED (exitCode);
    Q_UNUSED (exitStatus);

    QProcess * proc = static_cast<QProcess*> (sender());
    proc->deleteLater();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::proc_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED (exitCode);
    Q_UNUSED (exitStatus);
    UserMsg	um;

    if (dot_process != NULL)
    {
        dot_process->deleteLater();
        dot_process = NULL;
    }

    if (itm_img != NULL)
    {
        Q_ASSERT (&img_scene == itm_img->scene());
        img_scene.removeItem (itm_img);
        delete itm_img;
        itm_img = NULL;
    }

    itm_img = new QGraphicsPixmapItem(
                QPixmap( cache_tmp_dest_ ) ,
                NULL
                );
    img_scene.addItem (itm_img);

    if (s_.b_log_start_end_)
    {
        um.add( UserMsg::MSG_INFO,	QObject::tr(
                    "Process ended." )
                );
        um.show();
    }
    img_scene.update();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::proc_started ()
{
    if (s_.b_log_start_end_)
    {
        UserMsg	um;
        um.add( UserMsg::MSG_INFO,	QObject::tr(
                    "Process started." )
                );
        um.show();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::proc_readyError ()
{
    QProcess * proc = static_cast<QProcess*> (sender());

    if (proc != NULL)
    {
        UserMsg	um;
        um.add( UserMsg::MSG_ERROR,	QObject::tr(
                    proc->readAllStandardError() )
                );
        um.show();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::proc_readyOutput ()
{
    QProcess * proc = static_cast<QProcess*> (sender());

    if (dot_process != NULL)
    {
        UserMsg	um;
        um.add( UserMsg::MSG_INFO,	QObject::tr(
                    proc->readAllStandardOutput() )
                );
        um.show();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::proc_error ( QProcess::ProcessError error )
{
    QProcess * proc = static_cast<QProcess*> (sender());


    QString s_err;
    switch (error) {
    case QProcess::FailedToStart:	s_err = tr ("The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program."); break;
    case QProcess::Crashed:			s_err = tr ("The process crashed some time after starting successfully."); break;
    case QProcess::Timedout:		s_err = tr ("The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again."); break;
    case QProcess::WriteError:		s_err = tr ("An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel."); break;
    case QProcess::ReadError:		s_err = tr ("An error occurred when attempting to read from the process. For example, the process may not be running."); break;
    case QProcess::UnknownError:	s_err = tr ("An unknown error occurred. This is the default return value of error()."); break;
    default:						s_err = QString ("%1 error" ).arg( DOTEDITOR_PROJECT_NAME);
    }
    if (proc != NULL)
    {
        proc->deleteLater();
        if (proc == dot_process)
            dot_process = NULL;
    }

    UserMsg	um;
    um.add (UserMsg::MSG_ERROR,	s_err);
    um.show();

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool				ImplProg::perform			()
{

    stopDotProcess();

    QString s = GuiCtrl::editorText().trimmed();
    if (s.isEmpty())
    {
        /* remove image if one exists */
        if (itm_img != NULL)
        {
            img_scene.removeItem (itm_img);
            delete itm_img;
            itm_img = NULL;
        }

        return true;
    }

    applySettings ();

    /* save the content to a file */
    if (writeToTemp( s ) == false)
        return false;

    startDotProcess();

    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::ctxDisplay		(
        const QPoint & loc, const QPoint & glob_loc )
{
    Q_UNUSED (loc);
    UserMsg		um;

    QMenu	mnu;
    QAction	ac_copy_img (tr( "Save image as..." ), &mnu);

    if (itm_img != NULL)
    {
        ac_copy_img.setIcon (QIcon( ":/resources/images/save.png" ));
        mnu.addAction (&ac_copy_img);
        connect( &ac_copy_img, SIGNAL( triggered() ),
                 this, SLOT (saveImageAs() ));

        mnu.exec (glob_loc);
    }
    else
    {
        um.add( UserMsg::MSG_WARNING,	QObject::tr(
                    "No image in preview panel." )
                );
        um.show();
    }

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QAction *			ImplProg::action					()
{
    return Gui::MW::unique()->predefAct_Prog();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::saveImageAs				()
{
    UserMsg		um;
    if (itm_img != NULL)
    {
        QString fileName = QFileDialog::getSaveFileName (GuiCtrl::mainGui());
        if (!fileName.isEmpty())
        {
            if (itm_img->pixmap().save( fileName ) == false)
            {
                um.add( UserMsg::MSG_ERROR,	QObject::tr(
                            "ERROR!!! The image could not be saved as \n%1" )
                        .arg( fileName )
                        );
                um.show();
            }
            else
            {
                um.add( UserMsg::MSG_ERROR,	QObject::tr(
                            "The image was saved as\n%1" )
                        .arg( fileName )
                        );
                um.show();
            }
        }
    }
}
/* ========================================================================= */
#include "../../gui/mw.h"
/* ------------------------------------------------------------------------- */
void				ImplProg::applySettings		()
{

    /* get new temporary files */
    QString s_tmp =
#if QT_VERSION < 0x050000
            QDesktopServices::storageLocation(
                QDesktopServices::TempLocation );
#else
            QStandardPaths::standardLocations (
                QStandardPaths::TempLocation).at (0);
#endif

    QFile f;
    UserMsg um;
    QMap<QString,QString> special_vars;

    const QDateTime now = QDateTime::currentDateTime();

    QString s_mw_crt = Gui::MW::unique()->currentFile ();
    QFileInfo crt_file (s_mw_crt);
    QString s_crt_path;
    QString s_crt_name;
    if (s_mw_crt.isEmpty()) {
        static QLatin1String temp_file_pattern ("yyyyMMdd-hhmmsszzz");
        s_crt_path = s_tmp;
        s_crt_name = now.toString (temp_file_pattern);
    } else {
        if (!crt_file.absoluteDir().exists()) {
            crt_file.absoluteDir().mkpath (".");
        }
        s_crt_path = crt_file.absolutePath();
        s_crt_name = crt_file.baseName();
    }
    special_vars["ED_FILE_PATH"] = s_crt_path;
    special_vars["ED_FILE_NAME"] = s_crt_name;
    special_vars["DATE"] = now.toString ("yyyy-MM-dd");
    special_vars["TIME"] = now.toString ("hhmmss");
    special_vars["DATETIME"] = now.toString ("yyyy-MM-dd hh-mm-ss");
    special_vars["CD"] = QDir::currentPath ();


    /* remove previously used temporary files */
    if (cache_tmp_dest_.isEmpty() == false) {
        f.setFileName (cache_tmp_dest_);
        if (f.exists()) {
            f.remove();
        }
    }
    if (cache_tmp_src_.isEmpty() == false) {
        f.setFileName (cache_tmp_src_);
        if (f.exists()) {
            f.remove();
        }
    }

    cache_tmp_dest_ = s_.tmp_dest_;
    cache_tmp_src_ = s_.tmp_src_;

    static QLatin1String dollar ("$");
    static QLatin1String temp_str ("TEMP");
    QProcessEnvironment procenv = QProcessEnvironment::systemEnvironment();
    bool seen_temp = false;
    foreach (const QString & s_key, special_vars.keys()) {
        cache_tmp_dest_.replace (dollar + s_key, special_vars.value (s_key));
        cache_tmp_src_.replace (dollar + s_key, special_vars.value (s_key));
    }
    foreach (const QString & s_key, procenv.keys()) {
        cache_tmp_dest_.replace (dollar + s_key, procenv.value (s_key));
        cache_tmp_src_.replace (dollar + s_key, procenv.value (s_key));
        if (0 ==s_key.compare(temp_str, Qt::CaseInsensitive)) {
            seen_temp = true;
        }
    }

    if (!seen_temp) {
        cache_tmp_dest_.replace ("$TEMP", s_tmp);
        cache_tmp_src_.replace ("$TEMP", s_tmp);
    }

    bool b_opn; QFile * ff;

    if (cache_tmp_src_.contains("XXX")) {
        QTemporaryFile * tf_s = new QTemporaryFile (cache_tmp_src_);
        b_opn = tf_s->open ();
        ff = static_cast<QFile*>(tf_s);
    } else {
        QFile * tf_s = new QFile (cache_tmp_src_);
        b_opn = tf_s->open (QIODevice::ReadWrite);
        ff = tf_s;
    }
    if (!b_opn) {
        um.add( UserMsg::MSG_ERROR,	QObject::tr(
                    "The input file %1 is unusable!" )
                .arg (cache_tmp_src_)
                );
        cache_tmp_src_.clear();
    } else {
        cache_tmp_src_ = ff->fileName();
        ff->close();
    }
    delete ff; ff = NULL;
    special_vars["IN"] = cache_tmp_src_;
    um.add( UserMsg::MSG_INFO,	QObject::tr("Using input file: %1" )
            .arg( cache_tmp_src_ ));


    if (cache_tmp_dest_.contains("XXX")) {
        QTemporaryFile * tf_s = new QTemporaryFile (cache_tmp_dest_);
        b_opn = tf_s->open ();
        ff = static_cast<QFile*>(tf_s);
    } else {
        QFile * tf_s = new QFile (cache_tmp_dest_);
        b_opn = tf_s->open (QIODevice::ReadWrite);
        ff = tf_s;
    }
    if (!b_opn) {
        um.add( UserMsg::MSG_ERROR,	QObject::tr(
                    "The input file %1 is unusable!" )
                .arg (cache_tmp_dest_)
                );
        cache_tmp_dest_.clear();
    } else {
        cache_tmp_dest_ = ff->fileName();
        ff->close();
    }
    delete ff; ff = NULL;
    special_vars["OUT"] = cache_tmp_dest_;
    um.add( UserMsg::MSG_INFO,	QObject::tr("Using output file: %1" )
            .arg (cache_tmp_dest_));

    /* program and arguments */
    cache_s_arg_ = s_.s_arg_;
    foreach (const QString & s_key, special_vars.keys()) {
        cache_s_arg_.replace (dollar + s_key, special_vars.value (s_key));
    }
    foreach (const QString & s_key, procenv.keys()) {
        cache_s_arg_.replace (dollar + s_key, procenv.value (s_key));
    }

    QString s_prog = s_.s_process_;
    foreach (const QString & s_key, special_vars.keys()) {
        s_prog.replace (dollar + s_key, special_vars.value (s_key));
    }
    foreach (const QString & s_key, procenv.keys()) {
        s_prog.replace (dollar + s_key, procenv.value (s_key));
    }

    um.add (UserMsg::MSG_INFO, tr ("Generator program: %1").arg (s_prog));
    um.add (UserMsg::MSG_INFO, tr ("Arguments: %1").arg (cache_s_arg_));
    um.show();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool				ImplProg::writeToTemp			(
        const QString & src_tx )
{
    UserMsg	um;

    QFile * f = new QFile (cache_tmp_src_);
    if (f->open( QIODevice::WriteOnly | QIODevice::Text ) == false)
    {
        delete f;
        um.add( UserMsg::MSG_INFO,
                tr( "Failed to open temporary file %1" )
                .arg( cache_tmp_src_ )
                );

        um.show();
        return false;
    }
    QTextStream tx (f);
    tx << src_tx;
    f->flush();
    f->close();
    delete f;

    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::Stg::save				()
{
    QSettings stgs;
    stgs.setValue ("ImplProg/tmp_src", tmp_src_);
    stgs.setValue ("ImplProg/tmp_dest", tmp_dest_);
    stgs.setValue ("ImplProg/process_args", s_arg_);
    stgs.setValue ("ImplProg/process", s_process_);
    stgs.setValue ("ImplProg/log_start_end", b_log_start_end_);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				ImplProg::Stg::load				()
{
    QSettings stgs;
    tmp_src_ = stgs.value("ImplProg/tmp_src",
                          "$TEMP/dot_XXXXXXXX.dot" )
            .toString();
    tmp_dest_ = stgs.value("ImplProg/tmp_dest",
                           "$TEMP/dot_XXXXXXXX.png")
            .toString();
    s_arg_ = stgs.value("ImplProg/process_args",
                        "-Tpng -o $OUT $IN" )
            .toString();

    s_process_ = stgs.value("ImplProg/process",
                            "dot" )
            .toString();
    b_log_start_end_ = stgs.value("ImplProg/log_start_end",
                                  false )
            .toBool();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ImplProg::Stg		ImplProg::settings				()
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
void				ImplProg::setSettings		 (ImplProg::Stg & stg)
{
    if (uniq_ == NULL)
    {
        stg.save();
    }
    else
    {
        uniq_->s_ = stg;
        stg.save();
        uniq_->applySettings();
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

