/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!				\file		usermsg.h
				\date		june 2010
				\author		TNick

				\brief		Contains the definition for UserMsg class


*//*

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef		__USERMSG_INC__
#define		__USERMSG_INC__
#ifdef		__cplusplus
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <QVector>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QTime>


/*  INCLUDES    ============================================================ */
//
//
//
//

/**
*	@brief	 class that intermediates the reporting to user
*
*/
class DE_EXPORT UserMsg
{

	//
	//
	//
	//
	/*  DEFINITIONS    --------------------------------------------------------- */



public:

	/**
	*	@brief	 describes the types of individual messages
	*/
	enum	MsgType		{
		MSG_ERROR = 0,
		MSG_WARNING,
		MSG_INFO,
		MSG_DBG_ERROR,
		MSG_DBG_WARNING,
		MSG_DBG_INFO,

		MSG_MAX_TY
	};


	/**
	*	@brief	 flags telling what is shown and what is not
	*/
	enum	MsgVisible		{
		MVIZ_ERROR = 1 << MSG_ERROR,
		MVIZ_WARNING = 1 << MSG_WARNING,
		MVIZ_INFO = 1 << MSG_INFO,
		MVIZ_DBG_ERROR = 1 << MSG_DBG_ERROR,
		MVIZ_DBG_WARNING = 1 << MSG_DBG_WARNING,
		MVIZ_DBG_INFO = 1 << MSG_DBG_INFO,

		MVIZ_MAX = MVIZ_ERROR | MVIZ_WARNING | MVIZ_INFO | MVIZ_DBG_ERROR | MVIZ_DBG_WARNING | MVIZ_DBG_INFO,
		MVIZ_ALL = MVIZ_MAX
	};

	/**
	*	@brief	 describes the ways messages may be presented
	*/
	enum	MsgOutType	{

		/**
		*	@brief	 the output is disabled; messages are cached until proper time
		*/
		MSGTY_DISABLED = -1,

		/**
		*	@brief	 0 - pop-up message always;
		*/
		MSGTY_POPUP = 0,

		/**
		*	@brief	 1 - pop-up message when log window is closed
		*/
		MSGTY_LOG_OR_POPUP,

		/**
		*	@brief	 2 - log window; show it if hidden
		*/
		MSGTY_LOG_FORCED,

		/**
		*	@brief	 3 - log window; don't show it if hidden
		*/
		MSGTY_LOG,

		/**
		*	@brief	 for validation purposes
		*/
		MSGTY_MAX

	};

	/**
	*	@brief	 indexes in \b html_format_ string list
	*/
	enum	MsgFormat		{
		MGT_ERROR = MSG_ERROR,
		MGT_WARNING = MSG_WARNING,
		MGT_INFO = MSG_INFO,
		MGT_DBG_ERROR = MSG_DBG_ERROR,
		MGT_DBG_WARNING = MSG_DBG_WARNING,
		MGT_DBG_INFO = MSG_DBG_INFO,

		MGT_MESSAGE
	};


	/**
	*	@brief	 one individual message in a corelation structure
	*/
	struct	MsgEntry	{
		MsgType		type;
		QString		text;
	};

	/**
	*	@brief	 this is how we store the list of messages
	*/
	typedef	QVector<MsgEntry>		MsgList;


	/**
	*	@brief	 a cached message block
	*/
	struct	MsgCacheEntry	{

		/**
		*	@brief	 the ID of this class
		*/
		unsigned					id_;

		/**
		*	@brief	 the moment when was recieved
		*/
		QTime						time_;

		/**
		*	@brief	 the list of messages
		*/
		MsgList						msg_;

	};

	/**
	*	@brief	 this is how we store the list of message blocks
	*/
	typedef	QVector<MsgCacheEntry>		MsgCacheList;


private:



	struct	StatData	{

		/**
		*	@brief	 provides an unique ID for each instance
		*/
		unsigned			ID_factory;

		/**
		*	@brief	 the stream used by dead message logging
		*/
		QTextStream			stx_dead;

		/**
		*	@brief	 the file used by dead message logging
		*/
		QFile				file_dead;

		/**
		*	@brief	 names of the various types of messages
		*/
		QStringList			ty_names_;

		/**
		*	@brief	 the way messages are presented to the user
		*/
		MsgOutType			out_ty_;

		/**
		*	@brief	 the cache of messages that will be flush() -ed at some point
		*/
		MsgCacheList		cache_msg_;

		/**
		*	@brief	 the types of messages that we show
		*/
		MsgVisible			viz_lst_;

		/**
		*	@brief	 the format for messages
		*/
		QStringList			html_format_;


		StatData(){
			ID_factory = 1;
		}
	};

	/*  DEFINITIONS    ========================================================= */
	//
	//
	//
	//
	/*  DATA    ---------------------------------------------------------------- */

private:

	/**
	*	@brief	 the ID of this class
	*/
	unsigned					id_;

	/**
	*	@brief	 the list of messages
	*/
	MsgList						msg_;



	/**
	*	@brief	 common things
	*/
	static StatData *			d_;


	/*  DATA    ================================================================ */
	//
	//
	//
	//
	/*  FUNCTIONS    ----------------------------------------------------------- */


public:

	/**
	*	@brief	 the only constructor
	*/
	UserMsg			( );

	/**
	*	@brief	 destructor
	*/
	~UserMsg		( );

	/**
	*	@brief	 adds an message
	*/
	void			add				(
		MsgType			ty,
		QString			message
	 );

	/**
	*	@brief	 routine that actually shows the message based on
	*	current settings
	*/
	void			show				 ();

	/**
	*	@brief	 export the information it contains
	*/
	void			exportData			 (MsgCacheEntry & outp);

	/**
	*	@brief	 create html representation of the data
	*/
	QString			toHtml				 ();

	/**
	*	@brief	 number of stored messages
	*/
	int				count				() const
	{ return msg_.count(); }


private:

	/**
	*	@brief	 loads default fromatting settings
	*
	*	The function loads default settings in \b html_format_ and in settings
	*	storage. If \b b_forced is true, curent setting are overwritten,
	*	otherwise only empty ones are replaced
	*/
	static void			doDefaultFormat		 (bool b_forced = true);


public:


	/**
	*	@brief	 initialises the error system
	*/
	static void			startUp				 ();

	/**
	*	@brief	 discards the error system
	*/
	static void			finishUp			 ();

	/**
	*	@brief	 initialize the display type based on settings
	*
	*	when the application starts the output is disabled; it is later
	*	enabled by the front-end using this function
	*/
	static void			initialDisplay		 ();

	/**
	*	@brief	 starts the dead message logging system
	*/
	static void			initDeadLog			 ();

	/**
	*	@brief	 gets the string that coresponds to a message type
	*/
	static QString		tyName			 (MsgType ty)
	{ return d_->ty_names_[ty]; }

	/**
	*	@brief	 tells the output type
	*/
	static MsgOutType	outputType			()
	{ return d_->out_ty_; }

	/**
	*	@brief	 changes the output type
	*/
	static void			setOutputType		 (MsgOutType new_ty);

	/**
	*	@brief	 presents cached messages to the user
	*/
	static void			flush				 ();

	/**
	*	@brief	 tell if a certain MsgType is visible or not
	*/
	static bool			isTypeVisible	 (MsgType ty)
	{
		if ((ty >= 0) && (ty < MSG_MAX_TY))
			return ( d_->viz_lst_ & (1 << ty)) != 0;
		return false;
	}

	/**
	*	@brief	 get visible types
	*/
	static inline MsgVisible typesVisible	()
	{ return  d_->viz_lst_;}

	/**
	*	@brief	 set if a certain MsgType is visible or not
	*/
	static void			setTypeVisible		 (MsgType ty, bool b_viz);
	/**
	*	@brief	 set if a certain MsgType is visible or not
	*/
	static void			setTypeVisible		 (MsgVisible ty);

	/**
	*	@brief	 convert a MsgCacheList structure to html using \b html_format_
	*/
	static QString		cacheListToHtml		 (const MsgCacheList & c_list);

	/**
	*	@brief	 convert a MsgCacheEntry structure to html using \b html_format_
	*/
	static QString		cacheEntryToHtml	 (const MsgCacheEntry & c_entry);

	/**
	*	@brief	 convert a MsgList structure to html using \b html_format_
	*/
	static QString		msgListToHtml		 (const MsgList & m_list);

	/**
	*	@brief	 convert a MsgEntry structure to html using \b html_format_
	*/
	static QString		msgEntryToHtml		 (const MsgEntry & m_entry);

	/**
	*	@brief	 present information about an exception
	*/
	static void			fatalException		 ();


	/*  FUNCTIONS    =========================================================== */
	//
	//
	//
	//

};	/*	class	UserMsg	*/
//
//
//
//

#endif	//	__cplusplus
#endif // __USERMSG_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
