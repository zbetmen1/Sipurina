#include "tabWidget.h"
#include "ui_tabWidget.h"
#include<QtGui>
#include <iostream>
using namespace std;


tabWidget::tabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabWidget)
{
    ui->setupUi(this);
    ui->verticalLayout->setSpacing(0);
    ui->verticalLayout->setMargin(0);
    ui->horizontalLayout->setMargin(0);
    setControls();
    QPixmap * map = new QPixmap("images/konacan.png");
    ui->controls->setMask(map->mask());
}

void tabWidget::setControls(){

    closeButton = ui->close;
    minimizeButton = ui->minimize;
    maximizeButton = ui->maximize;

}

void tabWidget::setTitle(QString msg){
    ui->title->setFont(QFont("Times New Roman", -1, 10, false));
    ui->title->setText(msg);
}

QWidget* tabWidget::getSlotWidget(){
    return ui->slotWidget;
}

QWidget * tabWidget::getControlWidget(){
    return ui->controlWidget;
}

QWidget *tabWidget::getControl2Widget()
{
    return ui->control2Widget;
}

tabWidget::~tabWidget()
{
    delete ui;
}

