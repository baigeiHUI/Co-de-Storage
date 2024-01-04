#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include "addedgedlg.h"
#include "addnodedlg.h"

#include <math.h>

#include <QKeyEvent>
#include <QRandomGenerator>
#include <QDebug>
#include <QMessageBox>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, 250, 250); // 逻辑地址
    setScene(scene);
    setCacheMode(CacheBackground); // 缓存静态背景
    setViewportUpdateMode(BoundingRectViewportUpdate); // 设置视图的更新模式
    setRenderHint(QPainter::Antialiasing); // 抗锯齿
    setTransformationAnchor(AnchorUnderMouse); // 缩放时以鼠标为中心
    scale(qreal(1.5), qreal(1.5));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Automaton"));

}

// 主要用于文件恢复数据
void GraphWidget::addNode(QString name, int Cas, qreal x, qreal y) {
    Node *node = new Node(this, name, Cas);
    this->scene()->addItem(node);
    node->setPos(x, y);
}

void GraphWidget::addEdge(QString A, QString B, QString input) {
    Node *nodeA = nullptr, *nodeB = nullptr;
    foreach (Node * node, Node::getNodeList()) {
        if (node->getName() == A) {
            nodeA = node;
        }
        if (node->getName() == B) {
            nodeB = node;
        }
    }
    if (!nodeA || !nodeB) {
        qDebug() << "Err in addEdge";
        return;
    }
    this->scene()->addItem(new Edge(nodeA, nodeB, input));
}

void GraphWidget::setmode(QGRAPH_CAS newMode) {
    mode = newMode;
}


void GraphWidget::mousePressEvent(QMouseEvent *event) {
    switch (mode) {
    case QGRAPH_CAS::ADDNODE: { // 添加点
        addNodeDlg dlg;
        if (dlg.exec() == QDialog::Accepted) {
            if (dlg.getName().isEmpty()) {
                 QMessageBox::warning(this, tr("警告"), tr("输入不能为空!"));
                 return;
            }
            foreach (Node *tmp, Node::getNodeList())
                if (tmp->getName() == dlg.getName()) {
                    QMessageBox::warning(this, tr("警告"), tr("状态不能重复!"));
                    return;
                }
            Node *node = new Node(this, dlg.getName(), dlg.getCas());

            this->scene()->addItem(node);
            node->setPos(mapToScene(event->pos()));
        }
        break;
    }
    case QGRAPH_CAS::ADDEDGE1: { // 添加边， 顶点1
        QGraphicsItem *item = itemAt(event->pos());
        if (item->type() == QGraphicsItem::UserType + 1) {
            tmpNode1 = qgraphicsitem_cast<Node *>(item);
            qDebug() << "Get Edge 1 " << tmpNode1->getName();
            mode = QGRAPH_CAS::ADDEDGE2;
        }
        break;
    }
    case QGRAPH_CAS::ADDEDGE2: { // 添加边， 顶点2
        QGraphicsItem *item = itemAt(event->pos());
        if (item->type() == QGraphicsItem::UserType + 1) {
            tmpNode2 = qgraphicsitem_cast<Node *>(item);
            qDebug() << "Get Edge 2 " << tmpNode2->getName();
            addEdgeDlg dlg(tmpNode1->getName(), tmpNode2->getName(), nullptr);
            if (dlg.exec() == QDialog::Accepted) {
                if (dlg.getInputList().isEmpty()) {
                     QMessageBox::warning(this, tr("警告"), tr("输入不能为空!"));
                     return;
                }
                Edge *newEdge = new Edge(tmpNode1, tmpNode2, dlg.getInputList());
                scene()->addItem(newEdge);
                newEdge->adjust();
            }
            mode = QGRAPH_CAS::ADDEDGE1;
        }
        break;
    }
    case QGRAPH_CAS::DELETE: { // 删除
        QGraphicsItem *item = itemAt(event->pos());
        delete item;
        break;
    }
    case QGRAPH_CAS::MODIFY: { // 修改
        QGraphicsItem *item = itemAt(event->pos());
        if (item->type() == QGraphicsItem::UserType + 1) { // 修改点
            Node *tmp = qgraphicsitem_cast<Node *>(item);
            addNodeDlg dlg(tmp->getName(), tmp->getCas());
            if (dlg.exec() == QDialog::Accepted) {
               if (dlg.getName().isEmpty()) {
                    QMessageBox::warning(this, tr("警告"), tr("输入不能为空!"));
                    return;
               }
               foreach (Node *t, Node::getNodeList())
                   if (t->getName() == dlg.getName() && t != tmp) {
                       QMessageBox::warning(this, tr("警告"), tr("无效修改!"));
                       return;
                    }
               tmp->setName(dlg.getName());
               tmp->setCas(dlg.getCas());
               tmp->hide(); tmp->show();

            }
        } else if (item->type() == QGraphicsItem::UserType + 2) {  // 修改边
            Edge *tmp = qgraphicsitem_cast<Edge *>(item);
            addEdgeDlg dlg(tmp->sourceNode()->getName(), tmp->destNode()->getName(),
                           tmp->getInputList());
            if (dlg.exec() == QDialog::Accepted) {
                tmp->setInputList(dlg.getInputList());
            }
        }
        break;
    }
    case QGRAPH_CAS::MOVE:
    default:
        QGraphicsView::mousePressEvent(event);
    }
}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}








