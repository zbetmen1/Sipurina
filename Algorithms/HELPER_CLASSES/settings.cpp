#include "settings.h"
#include "ui_settings.h"
#include <QColorDialog>

settings::settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    ui->helpCHeck->setChecked(Node::VHelp());
    ui->heuristicCheck->setChecked(Node::VHeuristic());
    ui->idCheck->setChecked(Node::VId());
    ui->nameCheck->setChecked(Node::VName());
    ui->lineEdit->setText(QString::number(Node::_graphicRadius));
    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(applySettings()));
    connect(ui->cancelButton,SIGNAL(clicked()),this, SLOT(close()));
    connect(ui->SelectDefaultColor,SIGNAL(clicked()),this, SLOT(setColor()));
    this->setWindowTitle("Settings");
    setFixedSize(width(), height());
}

settings::~settings()
{
    delete ui;
}

void settings::applySettings()
{
    Node::setVHelp(ui->helpCHeck->isChecked());
    Node::setVHeuristic(ui->heuristicCheck->isChecked());
    Node::setVId(ui->idCheck->isChecked());
    Node::setVName(ui->nameCheck->isChecked());
    Node::_graphicRadius = ui->lineEdit->text().toInt();
    close();
}

void settings::setColor()
{
    QColorDialog * blah = new QColorDialog();
    blah->setStyleSheet("QColorDialog {background-color: rgb(140, 140, 140); color: white;}");
    blah->show();
    connect(blah,SIGNAL(colorSelected(QColor)),this,SLOT(applyColor(QColor)));
}

void settings::applyColor(QColor foo)
{
    Node::_chosenColor = foo;
}
