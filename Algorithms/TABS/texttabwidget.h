#ifndef TEXTTABWIDGET_H
#define TEXTTABWIDGET_H

#include"tabWidget.h"
#include<QTextDocument>
#include<QTextEdit>
#include<QLineEdit>
#include<QTextCursor>

class textTabWidget : public tabWidget
{
public:
    textTabWidget();
    ~textTabWidget();
    void readFile(QString path);
private slots:
    void playPauseRoutine() {}
    void textSearch(QString );
    void nextSlot();
    void previousSlot();
private:
    QTextDocument * m_textDocument;
    QTextEdit * m_textEdit;
    QLineEdit * m_textField;
    QPushButton * m_next;
    QPushButton * m_previous;
    QTextCursor * m_textDCursor;
    QTextCursor * m_textECursor;
};

#endif // TEXTTABWIDGET_H
