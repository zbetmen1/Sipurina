#include "entry.h"
#include "ui_entry.h"
#include<QFileDialog>
#include<QFile>

Entry::Entry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Entry)
{
    ui->setupUi(this);
    _fileChoose = ui->chooseFile;
    _filePath = ui->filePath;
    _fileCommit = ui->fileCommit;

    _randomNumber = ui->randomNubmer;
    _randomCommit = ui->randomGenerate;

    _handText = ui->handText;
    _handCommit = ui->handCommit;

    connect(_fileCommit,SIGNAL(clicked()),this,SLOT(fileCommitSlot()));
    connect(_fileChoose,SIGNAL(clicked()),this,SLOT(fileChoose()));
    connect(_randomCommit,SIGNAL(clicked()), this, SLOT(randomCommitSlot()));
    connect(_handCommit,SIGNAL(clicked()),this, SLOT(handCommitSlot()));
    ui->filePath->setDisabled(true);
}

QLineEdit *&Entry::getRandomInputLineEdit()
{
    return ui->randomNubmer;
}

QLineEdit *&Entry::getByHandInputLineEdit()
{
    return ui->handText;
}



void Entry::handCommitSlot(){
    _entry = _handText->text();
    emit entryDone();
    cleaningLady();
}

void Entry::randomCommitSlot(){
    _entry = QString("r ")+_randomNumber->text();
    emit entryDone();
    cleaningLady();
    /* FLAG: Random enter of data
     * Random enter of data must be implemented inside of structure!
     */
}

void Entry::fileChoose(){
    QFileDialog * blah = new QFileDialog();
    connect(blah ,SIGNAL(fileSelected(QString)),this,SLOT(filePath(QString)));
    blah->show();
}

void Entry::filePath(QString path){
    _filePath->setText(path);
}

void Entry::cleaningLady()
{
    ui->filePath->setText("");
    ui->randomNubmer->setText("");
    ui->handText->setText("");
}

void Entry::fileCommitSlot(){
    QFile * f = new QFile(_filePath->text());
    f->open(QIODevice::ReadOnly);
    _entry = QString(f->readAll());
    emit entryDone();
    cleaningLady();
}

Entry::~Entry()
{
    delete ui;
}
