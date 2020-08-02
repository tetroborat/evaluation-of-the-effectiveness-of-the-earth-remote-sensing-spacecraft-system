#ifndef C_LISTSC_H
#define C_LISTSC_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include "manager.h"

namespace Ui {
class C_listSC;
}

class C_listSC : public QWidget
{
    Q_OBJECT

public:
    QVector <C_SpaceCraft> *listKA;
    QVector <C_SpaceCraft> *listKA_usage = new QVector <C_SpaceCraft>;
    explicit C_listSC(QWidget *parent = 0);
    ~C_listSC();
    Ui::C_listSC *ui;

private:
    C_SpaceCraft *ka;

public slots:
    void click_check_KA(QTreeWidgetItem*item,int);
    void currentKA(QTreeWidgetItem*item,int);

private slots:
    void on_pushButton_add_SC_clicked();
    void fill_list_SC();
};

#endif // C_LISTSC_H
