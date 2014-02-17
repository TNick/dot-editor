/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			implprog.h
  \date			May 2012
  \author		TNick

  \brief		Contains the definition for ImplProg class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __IMPLPROG_INC__
#define __IMPLPROG_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<impl/implinterf.h>

#include	<QObject>
#include	<QProcess>
#include	<QGraphicsScene>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

class	QGraphicsPixmapItem;

/**
*	@brief	Implementation using external programs
*/
class ImplProg		: public QObject, public ImplInterf		{
	Q_OBJECT

	//
	//
	//
	//
	/*  DEFINITIONS    ----------------------------------------------------- */

public:

	/**
	*	@brief	settings for this implementation
	*/
	struct	Stg		{

		friend class ImplProg;

		/**
		*	@brief	arguments to use with the process
		*/
		QString				s_arg_;


		/**
		*	@brief	the (path and) name o the process to start
		*/
		QString				s_process_;


		/**
		*	@brief	path to source file
		*/
		QString				tmp_src_;


		/**
		*	@brief	path to destination file
		*/
		QString				tmp_dest_;


		/**
		*	@brief	log start and end of the process
		*/
		bool				b_log_start_end_;


	protected:

		/**
		*	@brief	dump to permanent storage
		*/
		void				save			( void );


		/**
		*	@brief	get from permanent storage
		*/
		void				load			( void );

	};


	/*  DEFINITIONS    ===================================================== */
	//
	//
	//
	//
	/*  DATA    ------------------------------------------------------------ */

private:

	/**
	*	@brief	cached settings
	*/
	Stg						s_;


	/**
	*	@brief	the process used to generate images
	*/
	QProcess *				dot_process;


	/**
	*	@brief	path to source file
	*/
	QString					cache_tmp_src_;


	/**
	*	@brief	path to destination file
	*/
	QString					cache_tmp_dest_;


	/**
	*	@brief	arguments to use with the process
	*/
	QString					cache_s_arg_;


	/**
	*	@brief	a single item is shown in scene: this one
	*/
	QGraphicsPixmapItem *	itm_img;


	/**
	*	@brief	the scene for image display
	*/
	QGraphicsScene			img_scene;





	/**
	*	@brief	the one and only instance
	*/
	static ImplProg *		uniq_;


	/*  DATA    ============================================================ */
	//
	//
	//
	//
	/*  FUNCTIONS    ------------------------------------------------------- */

private:


	/**
	*	@brief	constructor;
	*/
	ImplProg			( void );


	/**
	*	@brief	destructor;
	*/
	~ImplProg			( void );



public:

	/**
	*	@brief	use this method to get the singleton (may create it)
	*/
	static ImplProg *	unique					( bool b_create = true )
	{ if ( ( uniq_ == NULL ) && b_create ) new ImplProg(); return uniq_; }


	/**
	*	@brief	tell the settings that govern us
	*/
	static Stg			settings				( void );


	/**
	*	@brief	change the settings that govern us
	*/
	static void			setSettings				( Stg & stg );




	/**
	*	@brief	the process (path and name) to use
	*/
	inline QString		processName				( void ) const
	{ return s_.s_process_; }


	/**
	*	@brief	change the process (path and name) to use
	*/
	inline void			setProcessName			( const QString & s_new_val )
	{ s_.s_process_ = s_new_val; }


	/**
	*	@brief	the arguments for the process
	*/
	inline QString		procArgs				( void ) const
	{ return s_.s_arg_; }


	/**
	*	@brief	change the arguments for the process
	*/
	inline void			setProcArgs				( const QString & s_new_val )
	{ s_.s_arg_ = s_new_val; }


	/**
	*	@brief	the input file
	*/
	inline QString		tmpInputFile			( void ) const
	{ return s_.tmp_src_; }


	/**
	*	@brief	change the input file
	*/
	inline void			setTmpInputFile			( const QString & s_new_val )
	{ s_.tmp_src_ = s_new_val; }


	/**
	*	@brief	the output file
	*/
	inline QString		tmpOutputFile			( void ) const
	{ return s_.tmp_dest_; }


	/**
	*	@brief	change the output file
	*/
	inline void			setTmpOutputFile		( const QString & s_new_val )
	{ s_.tmp_dest_ = s_new_val; }


private:


	/**
	*	@brief	saves provided content to temporary file
	*/
	bool				writeToTemp				( const QString & src_tx );


	/**
	*	@brief	"Signal" that the settings have changed
	*/
	void				applySettings			( void );




public:


	/* ******************************************************************** */
	/** @name                   REQUIRED METHODS                            */
	/* ******************************************************************** */
	///@{


	/**
	*	@brief	Implementation is required to do its thing
	*/
	bool				perform					( void );


	/**
	*	@brief	context menu for graphical display
	*/
	void				ctxDisplay				(
			const QPoint &		loc,
			const QPoint &		glob_loc
			);

	/**
	*	@brief	the scene implemented by the implementation
	*/
	virtual QGraphicsScene*	scene				( void )
	{ return &img_scene; }


	/**
	*	@brief	associated action
	*/
	QAction*			action					( void );



	///@}
	/* ******************************************************************** */



private slots:



	/**
	*	@brief	informed that the process finished
	*/
	void		proc_finished			(
			int						exitCode,
			QProcess::ExitStatus	exitStatus
			);

	/**
	*	@brief	informed that the instance finished
	*/
	void		inst_finished			(
			int						exitCode,
			QProcess::ExitStatus	exitStatus
			);

	/**
	*	@brief	informed that the process started succesfully
	*/
	void		proc_started			( void );


	/**
	*	@brief	error occurs with the process
	*/
	void		proc_error				(
			QProcess::ProcessError	error
			);


	/**
	*	@brief	process has made new data available through its standard error channel
	*/
	void		proc_readyError			( void );


	/**
	*	@brief	process has made new data available through its standard output channel
	*/
	void		proc_readyOutput		( void );


	/**
	*	@brief	if the process is running it is stopped
	*/
	void		stopDotProcess			( void );


	/**
	*	@brief	if the process is running it is stopped
	*/
	void		startDotProcess			( void );


	/**
	*	@brief	save curently displayed images as ...
	*/
	void		saveImageAs				( void );




	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class ImplProg	*/

/*  CLASS    =============================================================== */
//
//
//
//
#endif // __IMPLPROG_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
