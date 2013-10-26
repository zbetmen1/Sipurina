#include "codetabwidget.h"
#include "CODE/codeeditor.h"

codeTabWidget::codeTabWidget()
{
    setTitle("SOURCE CODE");
    m_editor = new CodeEditor();
    getSlotWidget()->setLayout(new QVBoxLayout);
    m_editor->setReadOnly(true);
    getSlotWidget()->layout()->addWidget(m_editor);
    m_editor->setProperty("geometry", QRect(0,0,width(), height()));

}

codeTabWidget::~codeTabWidget()
{
    delete m_editor;
}
