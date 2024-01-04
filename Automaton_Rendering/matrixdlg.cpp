#include "matrixdlg.h"
#include "ui_matrixdlg.h"
#include "QMessageBox"
#include "node.h"
#include "edge.h"
#include <QStringList>
#include <QDebug>
#include <QRandomGenerator>



MatrixDlg::MatrixDlg(QVector<Node *> nodelist):
    QDialog(nullptr),
    ui(new Ui::MatrixDlg)
{
    ui->setupUi(this);
    QString s_state_set;//遍历每一个结点的状态
    QStringList sl_input_list;//字母集
    QString s_init_state;//唯一初态
    QString s_final_state_set;//终态
    //遍历nodeList中的所有结点
    for (Node * node : nodelist) {
        s_state_set += (s_state_set.isEmpty()?"":",") + node->getName();   //获取所有状态
        if (node->getCas() & 1) {           //获取初态!
            s_init_state = node->getName();
        }
        if (node->getCas() & 2) {
            s_final_state_set += (s_final_state_set.isEmpty()?"":",") + node->getName();  //获取所有终态!
        }

       //对每个结点相连的边进行遍历,筛选出以该结点为前置状态的结点
        foreach (const Edge *edge, node->edges()) {
            if (edge->sourceNode() == node) {
                QStringList inputlist = edge->getInputList().split(',');
                //此时inputlist包含该转换边对应的所有输入
                for (int i = 0; i < inputlist.size(); i++) {
                   if(!sl_input_list.contains(inputlist[i]))
                       sl_input_list.append(inputlist[i]);    //获取所有输入字母!
                }
            }
        }
    }
    model = new QStandardItemModel;
    ui->tableView->setModel(model);
    //设置行表头为输入字母
    model->setColumnCount(sl_input_list.size());
    for (int i = 0; i < sl_input_list.size(); i++)
        model->setHeaderData(i,Qt::Horizontal, sl_input_list[i]);
    //设置列表头为状态
    QStringList sl_state_set = s_state_set.split(",");
    model->setRowCount(sl_state_set.size());
    for (int i = 0; i < sl_state_set.size(); i++)
        model->setHeaderData(i, Qt::Vertical, sl_state_set[i]);

    foreach (Node * node, nodelist) {
        foreach (const Edge *edge, node->edges()) {
            if (edge->sourceNode() == node) {
                int idRow = 0;
                for (int i = 0; i < sl_state_set.size(); i++)
                    if (sl_state_set[i] == edge->sourceNode()->getName())
                        idRow = i;
                for (int idCol = 0; idCol < sl_input_list.size(); idCol++)
                    if (edge->getInputList().split(",").contains(sl_input_list[idCol]))
                        model->setItem(idRow, idCol, new QStandardItem(edge->destNode()->getName()));

            }
        }
    }

    ui->lineEdit_state_set->setText(s_state_set);
    ui->lineEdit_init_state->setText(s_init_state);
    ui->lineEdit_final_state_set->setText(s_final_state_set);
    ui->lineEdit_input_set->setText(sl_input_list.join(","));
}


MatrixDlg::~MatrixDlg()
{
    delete ui;
    delete model;
}

