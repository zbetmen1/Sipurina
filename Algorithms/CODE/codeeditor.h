#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
class Highlighter;

class CodeEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QString & fileName, QWidget *parent = 0);
    CodeEditor();
    friend class MainWindow;
    
signals:
    
public slots:
    void setFile(QString& fileName);
    void highlightLine(int lineNumber = 0);

private:
    Highlighter * m_cHighlighter;
    static const QString m_basePath;
    int m_lastHighlightedLine;
    static int constexpr m_lineLength= 500;

};

#endif // CODEEDITOR_H
