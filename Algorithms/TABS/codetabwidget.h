#ifndef CODETABWIDGET_H
#define CODETABWIDGET_H

#include"tabWidget.h"
#include "CODE/codeeditor.h"
#include "CODE/highlighter.h"

class codeTabWidget : public tabWidget
{
public:
    codeTabWidget();
    ~codeTabWidget();
    friend class MainWindow;
private slots:
    void playPauseRoutine() {}
private:
    CodeEditor* m_editor;
};

#endif // CODETABWIDGET_H
