#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QVector>
#include <abstractmaton.h>

enum class QGRAPH_CAS : unsigned int {
    MOVE     = 0x00,
    ADDNODE  = 0x01,
    ADDEDGE1 = 0x02,
    ADDEDGE2 = 0x03,
    DELETE   = 0x04,
    MODIFY   = 0x05
};


class Node;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = nullptr);

    void itemMoved();
    void setmode(QGRAPH_CAS newMode);
    void addNode(QString name, int Cas, qreal x, qreal y);
    void addEdge(QString A, QString B, QString input);

protected:
    void mousePressEvent(QMouseEvent *event) override;
private:
    int timerId = 0;
    QGRAPH_CAS mode = QGRAPH_CAS::ADDNODE;

    Node *tmpNode1, *tmpNode2; //*tmpInitState;
};

#endif // GRAPHWIDGET_H
