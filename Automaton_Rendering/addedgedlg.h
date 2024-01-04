#ifndef ADDEDGEDLG_H
#define ADDEDGEDLG_H

#include <QDialog>

namespace Ui {
class addEdgeDlg;
}

class addEdgeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit addEdgeDlg(QString nameA, QString nameB, QWidget *parent = nullptr);
    explicit addEdgeDlg(QString nameA, QString nameB, QString inputL, QWidget *parent = nullptr);
    ~addEdgeDlg();

    QString getInputList();

private:
    Ui::addEdgeDlg *ui;
};

#endif // ADDEDGEDLG_H
