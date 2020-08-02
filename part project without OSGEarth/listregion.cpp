#include "listregion.h"
#include "ui_listregion.h"

C_listregion::C_listregion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_listregion)
{
    ui->setupUi(this);
    list_region = C_manager().list_region;
    for (int var = 0; var < list_region->size(); ++var) {
        S_ground_object go_copy = list_region->operator [](var);
        list_region_usage->push_back(go_copy);
    }
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(current_region(QTreeWidgetItem*,int)));
    connect(ui->treeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),SLOT(change_item(QTreeWidgetItem*,int)));
    QStringList header = QString("  Долгота, гр.  Широта, гр.  Примечание").split("  ");
    ui->treeWidget->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->treeWidget->setHeaderLabels(header);
    ui->groupBox->setVisible(false);
    fill_list_region();
}

C_listregion::~C_listregion()
{
    delete ui;
}

void C_listregion::current_region(QTreeWidgetItem *item,int)
{
    if (ui->groupBox->isVisible())
    {
        for (int i = 0; i< list_region->size(); i++)
        {
            S_ground_object rs = list_region->operator [](i);
            if (rs.name==item->text(0))
            {
                ui->lineEdit_latitude->setText(QString::number(rs.latitude*180/M_PI));
                ui->lineEdit_longitude->setText(QString::number(rs.longitude*180/M_PI));
                ui->lineEdit_name->setText(rs.name);
                ui->plainTextEdit->setPlainText(rs.notice);
                ui->comboBox_type_region->setCurrentText(rs.type);
                break;
            }
        }
    }
    if (item->text(0) == " Добавить объект")
    {
        ui->treeWidget->header()->resizeSection(0,200);
        ui->lineEdit_longitude->clear();
        ui->lineEdit_latitude->clear();
        ui->lineEdit_name->clear();
        ui->plainTextEdit->clear();
        ui->comboBox_type_region->setCurrentText("");
        ui->groupBox->setVisible(true);
    }
}

void C_listregion::change_item(QTreeWidgetItem *item,int)
{
    if (item->checkState(0) == Qt::Checked)
        for (int var = 0; var < list_region->size(); ++var)
        {
            if (list_region->at(var).name == item->text(0))
            {list_region_usage->append(list_region->operator [](var));
            break;}
        }
    else
        for (int var = 0; var < list_region_usage->size(); ++var)
        {
            if (list_region_usage->at(var).name == item->text(0))
            {list_region_usage->remove(var);
            break;}
        }
}

void C_listregion::fill_list_region()
{
    ui->treeWidget->clear();
    ui->comboBox_type_region->clear();
    QVector <QString> types;
    bool check_type = false;
    for (int var = 1; var < ui->treeWidget->columnCount()-1; ++var)
        ui->treeWidget->resizeColumnToContents(var);
    ui->treeWidget->header()->resizeSection(0,400);
    ui->treeWidget->header()->resizeSection(3,600);
    for (int var = 0; var < list_region->size(); ++var)
    {
        for (int var1 = 0; var1 < types.size(); ++var1)
            if (list_region->at(var).type == types.at(var1))
                check_type = true;
        if (!check_type)
        {
            types.push_back(list_region->at(var).type);
            ui->comboBox_type_region->addItem(list_region->at(var).type,QVariant::Char);
        }
        check_type = false;
    }
    for (int var = 0; var < types.size(); ++var)
    {
        QTreeWidgetItem * item_parent = new QTreeWidgetItem();
        item_parent->setText(0,types.at(var));
        for (int i = 0;i < list_region->size();i++)
        {
            if (list_region->at(i).type == types.at(var))
            {
                S_ground_object rs = list_region->operator [](i);
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setCheckState(0,Qt::Unchecked);
                item->setText(0,rs.name);
                item->setText(1,QString::number(180/M_PI*rs.longitude));
                item->setText(2,QString::number(180/M_PI*rs.latitude));
                item->setText(3,QString(read_notice(rs.notice,95)));
                for (int var = 1; var < item->columnCount()-1; ++var)
                    item->setTextAlignment(var,Qt::AlignCenter);
                item->setTextAlignment(3,Qt::AlignJustify);
                for (int var = 0; var < list_region_usage->size(); ++var)
                    if (list_region->at(i).name == list_region_usage->at(var).name)
                    {
                        item->setCheckState(0,Qt::Checked);
                        break;
                    }
                item_parent->addChild(item);
            }
        }
        ui->treeWidget->addTopLevelItem(item_parent);
    }

    ui->treeWidget->sortItems(0,Qt::AscendingOrder);
    QTreeWidgetItem * item = new QTreeWidgetItem();
    item->setText(0," Добавить объект");
    ui->treeWidget->addTopLevelItem(item);
}

QString C_listregion::read_notice(QString notice, int width_item)
{
    QStringList notice_list = notice.split(" ",QString::SkipEmptyParts);
    QString result, intermediate_result;
    int number_of_words = 0;
    while (number_of_words<notice_list.size())
    {
        while (number_of_words<notice_list.size() && intermediate_result.size()+notice_list[number_of_words].size() < width_item)
        {
            intermediate_result.append(notice_list[number_of_words]+" ");
            result.append(notice_list[number_of_words]+" ");
            number_of_words ++;
        }
        intermediate_result.clear();
        if (number_of_words<notice_list.size())
            result.append("\n");
    }
    return result;
}

void C_listregion::on_pushButton_clicked()
{
    S_ground_object GO;
    GO.name = ui->lineEdit_name->text();
    GO.type = ui->comboBox_type_region->currentText();
    GO.longitude = ui->lineEdit_longitude->text().toDouble()/180*M_PI;
    GO.latitude = ui->lineEdit_latitude->text().toDouble()/180*M_PI;
    GO.notice = ui->plainTextEdit->toPlainText();
    ui->treeWidget->header()->resizeSection(0,400);
    list_region->append(GO);
    fill_list_region();
    ui->groupBox->setVisible(false);
}
