#include "addedgedlg.h"
#include "ui_addedgedlg.h"

addEdgeDlg::addEdgeDlg(QString nameA, QString nameB, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addEdgeDlg)
{
    ui->setupUi(this);
    ui->lineEdit_stA->setText(nameA);
    ui->lineEdit_edA->setText(nameB);
}

addEdgeDlg::addEdgeDlg(QString nameA, QString nameB, QString inputL, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addEdgeDlg)
{
    ui->setupUi(this);
    ui->lineEdit_stA->setText(nameA);
    ui->lineEdit_edA->setText(nameB);

    ui->lineEdit_inputA->setText(inputL);

}

addEdgeDlg::~addEdgeDlg()
{
    delete ui;
}
QString addEdgeDlg::getInputList() {
    return ui->lineEdit_inputA->text();
}




