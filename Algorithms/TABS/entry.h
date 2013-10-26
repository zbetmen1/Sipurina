#ifndef ENTRY_H
#define ENTRY_H

#include <QWidget>
#include<QPushButton>
#include<QLineEdit>

namespace Ui {
class Entry;
}

class Entry : public QWidget
{
    Q_OBJECT
    
public:
    explicit Entry(QWidget *parent = 0);
    QString getEntry() { return _entry; }
    QLineEdit *& getRandomInputLineEdit();
    QLineEdit *& getByHandInputLineEdit();
    ~Entry();
signals:
    void entryDone();
private slots:
    void fileCommitSlot();
    void randomCommitSlot();
    void handCommitSlot();
    void fileChoose();
    void filePath(QString path);
private:
    Ui::Entry *ui;
    QPushButton * _fileChoose;
    QLineEdit * _filePath;
    QPushButton * _fileCommit;
    QLineEdit * _randomNumber;
    QPushButton * _randomCommit;
    QLineEdit * _handText;
    QPushButton * _handCommit;
    QString _entry;
    void cleaningLady();
};

#endif // ENTRY_H
