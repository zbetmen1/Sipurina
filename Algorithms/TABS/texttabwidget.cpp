#include "texttabwidget.h"

#include<iostream>
#include<QFile>
#include<QTextEdit>
#include<QTextStream>
#include <QTextCursor>

textTabWidget::textTabWidget()
{
    setTitle("OPIS ALGORITMA");
    m_textEdit = new QTextEdit();
    m_textEdit->setReadOnly(true);
    m_textDocument = new QTextDocument();
    getSlotWidget()->setLayout(new QVBoxLayout);
    getSlotWidget()->layout()->addWidget(m_textEdit);

    m_textDCursor = new QTextCursor();
    m_textECursor = new QTextCursor();
    m_textField = new QLineEdit("");
    m_next = new QPushButton("Next");
    m_previous = new QPushButton("Previous");

    m_next->setFixedSize(80,30);
    m_previous->setFixedSize(80,30);
    m_textField->setFixedHeight(30);

    getControlWidget()->setLayout(new QHBoxLayout);
    getControlWidget()->layout()->setMargin(1);
    getControlWidget()->layout()->addWidget(m_textField);
    getControlWidget()->layout()->addWidget(m_next);
    getControlWidget()->layout()->addWidget(m_previous);

    connect(m_textField,SIGNAL(textChanged(QString)),this,SLOT(textSearch(QString)));
    connect(m_previous, SIGNAL(clicked()),this,SLOT(previousSlot()));
    connect(m_next,SIGNAL(clicked()),this,SLOT(nextSlot()));
}

textTabWidget::~textTabWidget()
{
    delete m_textDocument;
    delete m_textEdit;
    delete m_textField;
    delete m_next;
    delete m_previous;
    delete m_textDCursor;
    delete m_textECursor;
}


void textTabWidget::readFile(QString path){
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    m_textDocument->setPlainText(in.readAll());
    m_textEdit->setDocument(m_textDocument);
    m_textDocument->setDefaultFont(QFont("Times New Roman", -1, -1, true));
}


void textTabWidget::textSearch(QString text){
    m_textEdit->textCursor().clearSelection();
    m_textEdit->moveCursor(QTextCursor::Start);
    m_textEdit->find(text);
    m_textEdit->setStyleSheet("selection-color: red; selection-background-color: yellow;");
}

void textTabWidget::nextSlot(){
    if(!m_textEdit->find(m_textField->text())){
        m_textEdit->moveCursor(QTextCursor::Start);
        m_textEdit->find(m_textField->text());
    }
}

void textTabWidget::previousSlot(){
    if(!m_textEdit->find(m_textField->text(), QTextDocument::FindBackward)){
        m_textEdit->moveCursor(QTextCursor::End);
        m_textEdit->find(m_textField->text(), QTextDocument::FindBackward);
    }
}
