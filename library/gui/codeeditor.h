/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			codeeditor.h
  \date			Mar 2012
  \author		TNick

  \brief		Contains the definition for CodeEditor class


*//*


****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __CODEEDITOR_INC__
#define __CODEEDITOR_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <QPlainTextEdit>
#include <QObject>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/**
*	@brief	extends the QTextEdit
*/
class CodeEditor	: public QPlainTextEdit			{
	Q_OBJECT

	//
	//
	//
	//
	/*  DEFINITIONS    ----------------------------------------------------- */

	/*  DEFINITIONS    ===================================================== */
	//
	//
	//
	//
	/*  DATA    ------------------------------------------------------------ */

private:

	QWidget *		lineNumberArea;


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
	CodeEditor			( QWidget *parent = 0 );


	/**
	*	@brief	destructor;
	*/
	~CodeEditor			( void );


	void		lineNumberAreaPaintEvent		( QPaintEvent *event );

	int			lineNumberAreaWidth				( void );

protected:

	void		resizeEvent						( QResizeEvent *event );

	/**
	*	@brief	handles auto-indentation
	*/
	void		keyPressEvent					( QKeyEvent * e );


	/**
	*	@brief	zooming using this one
	*/
	void		wheelEvent		( QWheelEvent * event );


private slots:

	void		updateLineNumberAreaWidth		( int newBlockCount );

	void		highlightCurrentLine			( void );

	void		updateLineNumberArea			( const QRect &, int );



	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};	/*	class CodeEditor	*/

/*  CLASS    =============================================================== */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/**
*	@brief	extends the QTextEdit
*/
class LineNumberArea	: public QWidget			{
	Q_OBJECT

	//
	//
	//
	//
	/*  DEFINITIONS    ----------------------------------------------------- */

	/*  DEFINITIONS    ===================================================== */
	//
	//
	//
	//
	/*  DATA    ------------------------------------------------------------ */

private:


	CodeEditor *		codeEditor;

	/*  DATA    ============================================================ */
	//
	//
	//
	//
	/*  FUNCTIONS    ------------------------------------------------------- */

public:

	LineNumberArea			( CodeEditor * editor ) : QWidget(editor) {
		codeEditor = editor;
	}

	QSize				sizeHint		( void ) const
	{
		return QSize(codeEditor->lineNumberAreaWidth(), 0);
	}

protected:


	void				paintEvent		( QPaintEvent * event )
	{
		codeEditor->lineNumberAreaPaintEvent(event);
	}


	/*  FUNCTIONS    ======================================================= */
	//
	//
	//
	//

};
/*  CLASS    =============================================================== */
//
//
//
//

#endif // __CODEEDITOR_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
