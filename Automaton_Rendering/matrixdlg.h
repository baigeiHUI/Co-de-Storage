#ifndef MATRIXDLG_H
#define MATRIXDLG_H

#include <QDialog>
#include <QStandardItemModel>
#include "node.h"
#include "graphwidget.h"


namespace Ui {
class MatrixDlg;
}

class MatrixDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MatrixDlg(QVector<Node *> nodelist);

    ~MatrixDlg();
public:
    Ui::MatrixDlg *ui;
    QStandardItemModel *model;

};

#endif // MATRIXDLG_H
