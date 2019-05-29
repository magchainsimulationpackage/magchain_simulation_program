#include "dialogshowattradius.h"
#include "ui_dialogshowattradius.h"


DialogShowAttradius::DialogShowAttradius(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowAttradius)
{
    ui->setupUi(this);

}

DialogShowAttradius::~DialogShowAttradius()
{
    delete ui;
}

void DialogShowAttradius::on_pushButtonClose_clicked()
{
    this->close();
}

void DialogShowAttradius::slotShowAttradius(QString *str){

    ui->textEditAttradius->append(*str);

}
