#ifndef C_INPUT_H
#define C_INPUT_H

#include <QWidget>
#include <spacecraft.h>
#include <QTreeWidgetItem>
#include <listsc.h>
#include "manager.h"
namespace Ui {
class C_input;
}

class C_input : public QWidget
{
    Q_OBJECT

public:
    QVector <C_SpaceCraft> *listKA;
    explicit C_input(QWidget *parent = 0);
    Ui::C_input *ui;

    ~C_input();

private:
    C_SpaceCraft ka;

public slots:
    void currentType(QTreeWidgetItem*item, int column);
};

#endif // C_INPUT_H
