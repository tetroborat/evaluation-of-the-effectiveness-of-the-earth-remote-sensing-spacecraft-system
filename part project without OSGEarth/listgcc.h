#ifndef LISTGCC_H
#define LISTGCC_H

#include <QWidget>
#include <QListWidgetItem>
#include "manager.h"

namespace Ui {
class C_listgcc;
}

class C_listgcc : public QWidget
{
    Q_OBJECT

public:
    QVector < S_ground_object > *list_receptions_points;
    QVector < S_ground_object > *list_control_means;
    QVector < S_ground_object > *list_receptions_points_usage = new QVector <S_ground_object>;
    QVector < S_ground_object > *list_control_means_usage = new QVector <S_ground_object>;
    explicit C_listgcc(QWidget *parent = 0);
    ~C_listgcc();

private:
    Ui::C_listgcc *ui;

public slots:
    void currentType(QListWidgetItem*item);
    void currentObject(QListWidgetItem*item);
    void click_check_Object(QListWidgetItem*item);
};

#endif // LISTGCC_H
