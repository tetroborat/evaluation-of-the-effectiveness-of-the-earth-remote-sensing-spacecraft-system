#include "listgcc.h"
#include "ui_listgcc.h"

C_listgcc::C_listgcc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_listgcc)
{
    ui->setupUi(this);
    list_control_means = C_manager().list_control_means;
    list_receptions_points = C_manager().list_receptions_points;
    for (int var = 0; var < list_control_means->size(); ++var) {
        S_ground_object go_copy = list_control_means->operator [](var);
        list_control_means_usage->push_back(go_copy);
    }
    for (int var = 0; var < list_receptions_points->size(); ++var) {
        S_ground_object go_copy = list_receptions_points->operator [](var);
        list_receptions_points_usage->push_back(go_copy);
    }
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),SLOT(currentType(QListWidgetItem*)));
    connect(ui->listWidget_2,SIGNAL(itemClicked(QListWidgetItem*)),SLOT(currentObject(QListWidgetItem*)));
    connect(ui->listWidget_2,SIGNAL(itemChanged(QListWidgetItem*)),SLOT(click_check_Object(QListWidgetItem*)));
}

C_listgcc::~C_listgcc()
{
    delete ui;
}

void C_listgcc::currentType(QListWidgetItem *item)
{
    ui->listWidget_2->clear();
    if (item->text() == "Средства управления")
    {
        ui->groupBox_2->setTitle("Перечень средств управления");
        for (int var = 0; var < list_control_means->size(); ++var)
        {
            QListWidgetItem * item = new QListWidgetItem(list_control_means->at(var).name);
            item->setCheckState(Qt::Unchecked);
            for (int var1 = 0; var1 < list_control_means_usage->size(); ++var1)
                if (list_control_means_usage->at(var1).name == list_control_means->at(var).name)
                {
                    item->setCheckState(Qt::Checked);
                    break;
                }
            ui->listWidget_2->addItem(item);
        }
    }
    else
    {
        ui->groupBox_2->setTitle("Перечень пунктов приема");
        for (int var = 0; var < list_receptions_points->size(); ++var)
        {
            QListWidgetItem * item = new QListWidgetItem(list_receptions_points->at(var).name);
            item->setCheckState(Qt::Unchecked);
            for (int var1 = 0; var1 < list_receptions_points_usage->size(); ++var1)
                if (list_receptions_points_usage->at(var1).name == list_receptions_points->at(var).name)
                {
                    item->setCheckState(Qt::Checked);
                    break;
                }
            ui->listWidget_2->addItem(item);
        }
    }
}

void C_listgcc::currentObject(QListWidgetItem *item)
{
    ui->lineEdit_latitude->clear();
    ui->lineEdit_longitude->clear();
    S_ground_object GO;
    if (ui->groupBox_2->title() == "Перечень средств управления")
    {
        for (int var = 0; var < list_control_means->size(); ++var)
        {
            GO = list_control_means->operator [](var);
            if (GO.name == item->text())
            {
                ui->lineEdit_latitude->setText(QString::number(GO.latitude/M_PI*180));
                ui->lineEdit_longitude->setText(QString::number(GO.longitude/M_PI*180));
                break;
            }
        }
    }
    else
    {
        for (int var = 0; var < list_receptions_points->size(); ++var)
        {
            GO = list_receptions_points->operator [](var);
            if (GO.name == item->text())
            {
                ui->lineEdit_latitude->setText(QString::number(GO.latitude/M_PI*180));
                ui->lineEdit_longitude->setText(QString::number(GO.longitude/M_PI*180));
                break;
            }
        }
    }
}

void C_listgcc::click_check_Object(QListWidgetItem *item)
{
    if (item->checkState() == Qt::Checked)
        if (ui->groupBox_2->title() == "Перечень средств управления")
            for (int var = 0; var < list_control_means->size(); ++var)
            {
                if (list_control_means->at(var).name == item->text())
                {
                    list_control_means_usage->append(list_control_means->operator [](var));
                    break;
                }
            }
        else
            for (int var = 0; var < list_receptions_points->size(); ++var)
            {
                if (list_receptions_points->at(var).name == item->text())
                {
                    list_receptions_points_usage->append(list_receptions_points->operator [](var));
                    break;
                }
            }
    else
        if (ui->groupBox_2->title() == "Перечень средств управления")
            for(int i = 0; i< list_control_means_usage->size(); i++)
            {
                if (list_control_means_usage->at(i).name == item->text())
                {
                    list_control_means_usage->remove(i);
                    break;
                }
            }
        else
            for(int i = 0; i< list_receptions_points_usage->size(); i++)
                if (list_receptions_points_usage->at(i).name == item->text())
                {
                    list_receptions_points_usage->remove(i);
                    break;
                }
}
