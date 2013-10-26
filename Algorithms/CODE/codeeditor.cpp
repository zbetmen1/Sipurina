#include "codeeditor.h"
#include <QFont>
#include "highlighter.h"
#include <QFile>
#include <QScrollBar>
#include <iostream>

using std::cout;
using std::endl;

const QString CodeEditor::m_basePath("cFiles/");

CodeEditor::CodeEditor(QString &fileName, QWidget *parent) :
    QTextEdit(parent), m_cHighlighter(new Highlighter(document())), m_lastHighlightedLine(-1)
{
    /*Setting font, text color and calling fileOpen function*/
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(11);

    setFont(font);
    setTextColor(Qt::black);

    setBackgroundRole(QPalette::Background);
    setFile(fileName);

}

CodeEditor::CodeEditor()
    :QTextEdit(0), m_cHighlighter(new Highlighter(document())), m_lastHighlightedLine(-1)
{
/*Setting font, text color and calling fileOpen function*/
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(11);

    setFont(font);
    setTextColor(Qt::black);

    setBackgroundRole(QPalette::Background);
}

/*Function that:
    open file fileName
    parse code from file
    make new file with lineNumbers
    show new file
*/
void CodeEditor::setFile(QString& fileName)
{
    clear();
    setLineWidth(m_lineLength);
    QFile file(m_basePath + fileName);
    if (file.open(QFile::ReadOnly | QFile::Text))
        setPlainText(file.readAll());
    int numOfLines = document()->lineCount();
    QString parsedFile= "";
    for(int i = 0; i<numOfLines; i++){
       int currentLineLength = document()->findBlockByLineNumber(i).text().length();
       QString lineNum = QString::number(i);
       lineNum += ": ";
       QString blanks(m_lineLength - currentLineLength - lineNum.length(), ' ');
       parsedFile.append(lineNum);
       parsedFile.append(document()->findBlockByLineNumber(i).text());
       parsedFile.append(blanks);
       parsedFile.append("\n");
    }
    setText(parsedFile);
    toHtml();
    setReadOnly(true);
}

/*Function that highlight line with line number lineNumber*/
void CodeEditor::highlightLine(int lineNumber)
{
    textCursor().clearSelection();
    moveCursor(QTextCursor::Start);
    find(QString::number(lineNumber) + ": ", QTextDocument::FindWholeWords);

    QTextEdit::ExtraSelection highlight;
    highlight.cursor = textCursor();

    if(m_lastHighlightedLine != -1){
        highlight.cursor.setPosition((m_lastHighlightedLine+1)*m_lineLength, QTextCursor::MoveAnchor);
        highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
        highlight.format.clearBackground();
    }

    highlight.cursor.setPosition((lineNumber+1)*m_lineLength, QTextCursor::MoveAnchor);
    highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
    highlight.format.setBackground( Qt::yellow );

    QList<QTextEdit::ExtraSelection> extras;
    extras << highlight;
    setExtraSelections( extras );
}

