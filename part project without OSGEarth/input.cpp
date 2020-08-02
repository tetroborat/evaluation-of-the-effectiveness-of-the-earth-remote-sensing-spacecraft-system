#include "input.h"
#include "ui_input.h"
#include "spacecraft.h"
#include "listsc.h"
#include "ui_listsc.h"
#include <QDebug>
#include "manager.h"
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"


C_input::C_input(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_input)
{
    ui->setupUi(this);
    //connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(currentType(QTreeWidgetItem*,int)));
    //ui->treeWidget->hideColumn(1);
    listKA = ui->page_SC->listKA;
}


C_input::~C_input()
{
    delete ui;
}

void C_input::currentType(QTreeWidgetItem * item, int column)
{
    qDebug()<<item->text(1);
    if (item->text(1)=="RV") ui->stackedWidget->setCurrentWidget(ui->page_region);
    if (item->text(1)=="OEN" || item->text(1)=="RLN")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_SC);
        if (item->text(1)=="OEN")
            ui->page_SC->ui->groupBox->setTitle("Космические аппараты ОЭР");
        else
            ui->page_SC->ui->groupBox->setTitle("Космические аппараты РТР");
        ui->page_SC->ui->treeWidget->clear();
        ui->stackedWidget->setCurrentWidget(ui->page_SC);
        for(int i = 0; i< listKA->size(); i++)
        {
            ka =  listKA->operator [](i);
            if (ka.type==item->text(1))
            {
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(column,ka.name);
                item->setCheckState(column,Qt::Checked);
                ui->page_SC->ui->treeWidget->addTopLevelItem(item);
            }
        }
    }
}
