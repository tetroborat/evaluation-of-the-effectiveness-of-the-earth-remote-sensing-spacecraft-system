#include "listsc.h"
#include "ui_listsc.h"
#include <QDebug>
#include "input.h"
#include "ui_input.h"
#include "manager.h"
#include "convert.h"
#include "calculation.h"

C_listSC::C_listSC(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_listSC)
{
    ui->setupUi(this);
    listKA = C_manager().listKA;
    for (int var = 0; var < listKA->size(); ++var) {
        C_SpaceCraft sc_copy = listKA->operator [](var);
        listKA_usage->append(sc_copy);
    }
    connect(ui->treeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),SLOT(click_check_KA(QTreeWidgetItem*,int)));
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(currentKA(QTreeWidgetItem*,int)));
    QStringList header = QString("   Большая\nполуось,\nкм  Эксцентриситет  \nНаклонение,\nгр.  Прямое\nвосхождение,\nгр.  Аргумент\nшироты,\nгр.  Время прохождения\nперигея,\nсек  Момент\nизмерения").split("  ");
    ui->treeWidget->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->treeWidget->setSortingEnabled(true);
    ui->treeWidget->setHeaderLabels(header);
    ui->groupBox_3->setVisible(false);
    fill_list_SC();
}

C_listSC::~C_listSC()
{
    delete ui;
}

void C_listSC::click_check_KA(QTreeWidgetItem *item,int)
{
    if (item->checkState(0) == Qt::Checked)
    {
        for (int var = 0; var < listKA->size(); ++var)
            if (listKA->at(var).name == item->text(0))
            {
                listKA_usage->append(listKA->operator [](var));
                break;
            }
    }
    else
        for(int i = 0; i< listKA_usage->size(); i++)
            if (listKA_usage->at(i).name == item->text(0))
            {
                listKA_usage->remove(i);
                break;
            }
}

void C_listSC::currentKA(QTreeWidgetItem *item,int)
{
    if (ui->groupBox_3->isVisible())
    {
        for(int i = 0; i< listKA->size(); i++)
        {
            ka =  &listKA->operator [](i);
            if (ka->name==item->text(0))
            {
                ui->lineEdit_i->setText(QString::number(ka->KEO.i*180./M_PI));
                ui->lineEdit_Om->setText(QString::number(ka->KEO.Om*180./M_PI));
                ui->lineEdit_a->setText(QString::number(ka->KEO.a));
                ui->lineEdit_e->setText(QString::number(ka->KEO.e));
                ui->lineEdit_om->setText(QString::number(ka->KEO.om*180./M_PI));
                ui->lineEdit_tau->setText(QString::number(ka->KEO.tau));
                ui->dateTimeEdit_toBD->setDateTime(ka->KEO.datetime);
                ui->lineEdit_name->setText(ka->name);
                ui->comboBox_type_SC->setCurrentText(ka->type);
                break;
            }
        }
    }
    if (item->text(0) == "Добавить КА")
    {
        ui->treeWidget->header()->resizeSection(0,300);
        ui->lineEdit_name->clear();
        ui->comboBox_type_SC->setCurrentText("");
        ui->dateTimeEdit_toBD->setDateTime(QDateTime::currentDateTime());
        ui->lineEdit_i->clear();
        ui->lineEdit_a->clear();
        ui->lineEdit_e->clear();
        ui->lineEdit_om->clear();
        ui->lineEdit_Om->clear();
        ui->lineEdit_tau->clear();
        ui->groupBox_3->setVisible(true);
    }
}

void C_listSC::on_pushButton_add_SC_clicked()
{
    C_SpaceCraft sc;
    S_KEO keo;
    keo.a = ui->lineEdit_a->text().toDouble();
    keo.e = ui->lineEdit_e->text().toDouble();
    keo.tau = ui->lineEdit_tau->text().toDouble();
    keo.Om = ui->lineEdit_Om->text().toDouble()/180.*M_PI;
    keo.om = ui->lineEdit_om->text().toDouble()/180.*M_PI;
    keo.i = ui->lineEdit_i->text().toDouble()/180.*M_PI;
    keo.datetime = ui->dateTimeEdit_toBD->dateTime();
    sc.KEO = keo;
    sc.name = ui->lineEdit_name->text();
    sc.ukhod = C_calculation::VekUkhod(keo);
    sc.type = ui->comboBox_type_SC->currentText();
    listKA->append(sc);
    fill_list_SC();
    ui->groupBox_3->setVisible(false);
}

void C_listSC::fill_list_SC()
{
    ui->treeWidget->clear();
    QVector <QString> types;
    bool check_type = false;
    ui->comboBox_type_SC->clear();
    for (int var = 0; var < listKA->size(); ++var)
    {
        for (int var1 = 0; var1 < types.size(); ++var1)
            if (listKA->at(var).type == types.at(var1))
                check_type = true;
        if (!check_type)
        {
            types.push_back(listKA->at(var).type);
            ui->comboBox_type_SC->addItem(listKA->at(var).type,QVariant::Char);
        }
        check_type = false;
    }
    for (int var = 0; var < types.size(); ++var)
    {
        QTreeWidgetItem * item_parent = new QTreeWidgetItem();
        item_parent->setText(0,types.at(var));
        for(int i = 0; i< listKA->size(); i++)
            if (listKA->at(i).type == types.at(var))
            {
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(0,listKA->at(i).name);
                item->setText(1,QString::number(listKA->at(i).KEO.a));
                item->setText(2,QString::number(listKA->at(i).KEO.e));
                item->setText(3,QString::number(listKA->at(i).KEO.i*180/M_PI));
                item->setText(4,QString::number(listKA->at(i).KEO.Om*180/M_PI));
                item->setText(5,QString::number(listKA->at(i).KEO.om*180/M_PI));
                item->setText(6,QString::number(listKA->at(i).KEO.tau));
                item->setText(7,listKA->at(i).KEO.datetime.toString("hh:mm:ss dd.MM.yyyy"));
                item->setCheckState(0,Qt::Unchecked);
                for (int var = 1; var < item->columnCount(); ++var)
                    item->setTextAlignment(var,Qt::AlignHCenter);
                for (int var = 0; var < listKA_usage->size(); ++var)
                    if (listKA->at(i).name == listKA_usage->at(var).name)
                    {
                        item->setCheckState(0,Qt::Checked);
                        break;
                    }
                item_parent->addChild(item);
            }
        ui->treeWidget->addTopLevelItem(item_parent);
    }
    for (int var = 1; var < ui->treeWidget->columnCount()+1; ++var) {
        ui->treeWidget->resizeColumnToContents(var);
    }
    QTreeWidgetItem * item = new QTreeWidgetItem();
    item->setText(0,"Добавить КА");
    ui->treeWidget->addTopLevelItem(item);
    ui->treeWidget->header()->resizeSection(0,600);
}
