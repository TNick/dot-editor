/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			syntax.cc
  \date			Mar 2012
  \author		TNick

  \brief		Contains the implementation of Syntax class


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


#include	<syntax.h>
#include	<keywords.h>

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

/*  DATA    ================================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
Syntax::Syntax	( QTextDocument * parent )
	: QSyntaxHighlighter( parent )
{

	const char ** iter;
	HighlightingRule rule;

	keywordFormat.setForeground(Qt::darkBlue);
	keywordFormat.setFontWeight(QFont::Bold);
	iter = &key_words[0];
	while ( *iter != NULL )
	{
		rule.pattern = QRegExp( QString( "\\b%1\\b" ).arg( *iter ) );
		rule.format = keywordFormat;
		highlightingRules.append(rule);

		iter++;
	}


	keywordFormat.setForeground(Qt::magenta);
	keywordFormat.setFontWeight(QFont::DemiBold);
	iter = &col_names[0];
	while ( *iter != NULL )
	{
		rule.pattern = QRegExp( QString( "\\b%1\\b" ).arg( *iter ) );
		rule.format = keywordFormat;
		highlightingRules.append(rule);

		iter++;
	}



	classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\b" "Q[A-Za-z]+\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);

	singleLineCommentFormat.setForeground(Qt::red);
	rule.pattern = QRegExp("//[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::red);

	quotationFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("\".*\"");
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	functionFormat.setFontItalic(true);
	functionFormat.setForeground(Qt::blue);
	rule.pattern = QRegExp("\\b" "[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);

	operatorsFormat.setFontItalic(false);
	operatorsFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp( "[\\{\\}\\[\\],=]" );
	rule.format = operatorsFormat;
	highlightingRules.append( rule );



	commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Syntax::~Syntax	( void )
{
	/* stub */
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void				Syntax::highlightBlock	( const QString & text )
{

	foreach (HighlightingRule rule, highlightingRules)
	{
		QRegExp expression(rule.pattern);
		int index = text.indexOf(expression);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = text.indexOf(expression, index + length);
		}
	}
	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = text.indexOf(commentStartExpression);

	while (startIndex >= 0)
	{
		int endIndex = text.indexOf(commentEndExpression, startIndex);
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} else {
			commentLength = endIndex - startIndex
					+ commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = text.indexOf(commentStartExpression,
								  startIndex + commentLength);
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
