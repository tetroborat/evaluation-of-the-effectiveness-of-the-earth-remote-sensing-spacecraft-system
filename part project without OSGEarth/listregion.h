#ifndef LISTREGION_H
#define LISTREGION_H

#include <QWidget>
#include <QTreeWidgetItem>
#include "manager.h"


namespace Ui {
class C_listregion;
}

class C_listregion : public QWidget
{
    Q_OBJECT

public:
    QVector <S_ground_object> *list_region;
    QVector <S_ground_object> *list_region_usage = new QVector <S_ground_object>;
    explicit C_listregion(QWidget *parent = 0);
    ~C_listregion();
    Ui::C_listregion *ui;

private slots:
    void current_region(QTreeWidgetItem*item,int);
    void change_item(QTreeWidgetItem*item,int);
    void fill_list_region();
    QString read_notice(QString notice, int width_item);

    void on_pushButton_clicked();
};

#endif // LISTREGION_H
