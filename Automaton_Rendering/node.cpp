#include "edge.h"
#include "node.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QtDebug>
#include <graphwidget.h>

//nodeList用于记录当前所有结点
QVector<Node *> Node::nodeList;
//初始化结点
Node::Node(GraphWidget *graphWidget, QString name, int nodeCas)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable); // 可被鼠标拖动
    setFlag(ItemSendsGeometryChanges); // itemChange()通知发生位置移动
    setCacheMode(DeviceCoordinateCache); // 加速渲染
    setZValue(10); // 放上层 容易点到
    m_nodeName = name;
    m_nodeCas = nodeCas;
    nodeList.push_back(this);
}

Node::~Node() {
    nodeList.removeAll(this);
    qDeleteAll(edgeList);
    edgeList.clear();
}

void Node::setName(QString name) {
    m_nodeName = name;
}
QString Node::getName() {
    return m_nodeName;
}

void Node::setCas(int newCas) {
    m_nodeCas = newCas;
}

int Node::getCas() {
    return m_nodeCas;
}

void Node::addEdge(Edge *edge) {
    edgeList << edge;
    edge->adjust();
}

void Node::removeEdge(Edge *edge) {
    edgeList.removeAll(edge);
    qDebug() << "rmEdge" << edgeList.size() << " \n";
}


QVector<Edge *> Node::edges() const {
    return edgeList;
}

QVector<Node *> Node::getNodeList() {
    return nodeList;
}

// 根据newPos更新位置
bool Node::advancePosition()
{
    if (newPos == pos())
        return false;
    setPos(newPos);
    return true;
}

//paint 绘制的图像必须在 boundingRect 函数之中
QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

// 画出圆点
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    painter->setPen(QPen(Qt::black, 0));
    painter->setBrush(Qt::white);
    //若既不是初态也不是终态
    painter->drawEllipse(-10, -10, 20, 20);

    if (m_nodeCas & 1) { // 初态

        painter->setBrush(Qt::red);
        painter->drawEllipse(-10, -10, 20, 20);
    }

    if (m_nodeCas & 2) { // 终态
       painter->drawEllipse(-7, -7, 14, 14);
    }

    QRectF textRect(-10, -10, 20, 20);
    painter->setPen(Qt::black);

    int flags = Qt::TextWordWrap | Qt::AlignCenter; // 自动换行，中间对齐
    painter->drawText(textRect, flags, m_nodeName);
}

// 拖动点位置, 更新相连的边
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
       for (Edge *edge : qAsConst(edgeList))
           edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

