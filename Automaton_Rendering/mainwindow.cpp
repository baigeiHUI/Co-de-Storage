#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphwidget.h"
#include "matrixdlg.h"
#include "node.h"
#include "edge.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gwidget = new GraphWidget;
    this->setCentralWidget(gwidget);
    ui->dockWidget->setStyleSheet(
            //设置按钮样式
            "QPushButton {"
            "border-radius: 12px;"
            "border: 1px solid #000000;"
            "color: #000000;"
            "}"

            "QPushButton:hover{"
            "border-radius: 12px;"
            "border: 1px solid #000000; "
            "background-color: qlineargradient(spread:pad,  x1:0, x2:0, y1:0, y2:1,"
            " stop: 0 rgba(200,220,240,255),"
            " stop: 0.504 rgba(173,216,230,255),"
            " stop: 0.505 rgba(171,215,225,255),"
            "stop: 1 rgba(200,220,240,255));"
            "}"

            "QPushButton:pressed{"
            " border-radius: 12px;"
            "background-color: qlineargradient(spread:pad,  x1:0, x2:0, y1:0, y2:1,"
            " stop: 0 rgba(200,220,240,255),"
            " stop: 0.504 rgba(230,230,230,255),"
            " stop: 0.505 rgba(230,230,230,255),"
            "stop: 1 rgba(200,220,225,255));"
            " border: 1px solid #000000;  "

            "}"

           );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButton_move_toggled(bool checked)
{
    if (checked)
        gwidget->setmode(QGRAPH_CAS::MOVE);
}

void MainWindow::on_radioButton_addNode_toggled(bool checked)
{
    if (checked)
        gwidget->setmode(QGRAPH_CAS::ADDNODE);
}

void MainWindow::on_radioButton_addEdge_toggled(bool checked)
{
    if (checked)
        gwidget->setmode(QGRAPH_CAS::ADDEDGE1);
}

void MainWindow::on_radioButton_delete_toggled(bool checked)
{
    if (checked)
        gwidget->setmode(QGRAPH_CAS::DELETE);
}

void MainWindow::on_radioButton_modify_toggled(bool checked)
{
    if (checked)
        gwidget->setmode(QGRAPH_CAS::MODIFY);
}


// 保存为文件
void MainWindow::on_pushButton_toFile_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,tr("保存文件"),"/home/test");
    qDebug()<<filePath;
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly)) {
           QMessageBox::warning(this, tr("打开文件错误"),"");
    }
    QTextStream stream(&file);

    auto nodeList = Node::getNodeList();
    stream << nodeList.size() << "\n";
    for (Node *node : qAsConst(nodeList)) {
       stream << node->getName() << " " << node->getCas() << " "
              << node->pos().x() << " " << node->pos().y() << "\n";
    }
    int cntEdge = 0;
    foreach (const Node *node, qAsConst(nodeList)) {
        foreach (const Edge *edge, node->edges()) {
            if (edge->sourceNode() == node) cntEdge++;
        }
    }
    stream << cntEdge << "\n";
    foreach (Node *node, nodeList) {
        foreach (const Edge *edge, node->edges()) {
            if (edge->sourceNode() == node) {
                stream << edge->sourceNode()->getName() << " " << edge->destNode()->getName() << " "
                       << edge->getInputList() <<  "\n";
            }
        }
    }
    file.close();
}

// 从文件导入
void MainWindow::on_pushButton_fromFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("打开文件"),"/home/test");
    qDebug()<<filePath;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("打开文件错误"), "");
    }
    QTextStream stream(&file);
    int cntNode = 0, cntEdge = 0;
    stream >> cntNode;
    for (int i = 0; i < cntNode; i++) {
        QString name; int Cas; qreal x; qreal y;
        stream >> name >> Cas >> x >> y;
        gwidget->addNode(name, Cas, x, y);
        qDebug() << name << " " << Cas << " " << x << " " << y <<"\n";
    }

    stream >> cntEdge;
    for (int i = 0; i < cntEdge; i++) {
        QString A, B, input;
        stream >> A >> B >> input;
        qDebug() << A << " " << B << " " << input << " " <<"\n";
        gwidget->addEdge(A, B, input);
    }
    file.close();
}

// 变矩阵
void MainWindow::on_pushButton_toMatrix_clicked()
{
    MatrixDlg dlg(Node::getNodeList());
    dlg.exec();
}

