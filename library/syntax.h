/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			syntax.h
  \date			Mar 2012
  \author		TNick

  \brief		Contains the definition for SyntaxC class


*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __SYNTAXC_INC__
#define __SYNTAXC_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include	<QSyntaxHighlighter>
#include	<QHash>
#include	<QTextCharFormat>

class QTextDocument;

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */


class Syntax	: public QSyntaxHighlighter 			{
	/*Q_OBJECT*/

	//
	//
	//
	//
	/*  DEFINITIONS    ----------------------------------------------------- */

	struct HighlightingRule
	{
		QRegExp			pattern;
		QTextCharFormat	format;
	};

	/*  DEFINITIONS    ===================================================== */
	//
	//
	//
	//
	/*  DATA    ------------------------------------------------------------ */

private:


	QVector<HighlightingRule>	highlightingRules;

	QRegExp						commentStartExpression;
	QRegExp						commentEndExpression;

	QTextCharFormat				keywordFormat;
	QTextCharFormat				classFormat;
	QTextCharFormat				singleLineCommentFormat;
	QTextCharFormat				multiLineCommentFormat;
	QTextCharFormat				quotationFormat;
	QTextCharFormat				functionFormat;
	QTextCharFormat				operatorsFormat;


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
	Syntax			 (QTextDocument * parent);


	/**
	*	@brief	destructor;
	*/
	virtual				~Syntax		 ();


protected:


	/**
	*	@brief	perfrms highliting
	*/
	void				highlightBlock		(
			const QString &			text
			)  ;


	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class SyntaxC	*/

/*  CLASS    =============================================================== */
//
//
//
//


#endif // __SYNTAXC_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
