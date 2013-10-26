#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include"DATA_STRUCTURES/node.h"

namespace Ui {
class settings;
}

class settings : public QWidget
{
    Q_OBJECT
    
public:
    explicit settings(QWidget *parent = 0);
    ~settings();
private slots:
    void applySettings();
    void setColor();
    void applyColor(QColor foo);

private:
    Ui::settings *ui;
};

#endif // SETTINGS_H
