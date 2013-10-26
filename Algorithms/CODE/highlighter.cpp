#include <QtGui>

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{

    HighlightingRule rule;

    /*Making pattern for keywords and adjusting color for them*/
    keywordFormat.setForeground(Qt::darkMagenta);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    /*Making pattern for loops*/
    loopFormat.setForeground(Qt::black);
    loopFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns2;
    keywordPatterns2 << "\\bif\\b" << "\\bfor\\b" << "\\bwhile\\b"
                    << "\\bdo\\b" << "\\bswitch\\b" << "\\bcase\\b"
                    << "\\bdefault\\b" << "\\belse\\b";
    foreach (const QString &pattern, keywordPatterns2) {
        rule.pattern = QRegExp(pattern);
        rule.format = loopFormat;
        highlightingRules.append(rule);
    }

    /*Making pattern for preprocessor directives*/
    preprocessorDirectivesFormat.setForeground(Qt::darkGreen);
    QStringList keywordPatterns3;
    keywordPatterns3 << "\\binclude\\b" << "\\bdefine\\b" << "\\bifdef\\b"
                    << "\\bifndef\\b" << "\\bendif\\b" << "\\btypedef\\b" <<"#";
    foreach (const QString &pattern, keywordPatterns3) {
        rule.pattern = QRegExp(pattern);
        rule.format = preprocessorDirectivesFormat;
        highlightingRules.append(rule);
    }

    /*Making pattern for upper case constants*/
    constants.setForeground(Qt::darkCyan);
    rule.pattern = QRegExp("\\b[A-Z_]+\\b");
    rule.format = constants;
    highlightingRules.append(rule);

    /*Making pattern for single line comment*/
    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("[ ]*//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    /*Making pattern for multi line comment*/
    multiLineCommentFormat.setForeground(Qt::darkYellow);

    /*Making pattern for strings*/
    quotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    /*Setting start and end expression for comments*/
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");

    /*Making pattern for numbers*/
    numbers.setForeground(QColor(247, 0, 119));
    rule.pattern = QRegExp("\\b[0-9]+:");
    rule.format = numbers;
    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{

    /*Coloring comments:*/
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
    /*Coloring the rest*/
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}

