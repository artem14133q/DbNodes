#ifndef RELATIONMAKER_H
#define RELATIONMAKER_H

#include <QMainWindow>
#include <QWidget>
#include <QVector>
#include <QPair>
#include <noderow.h>
#include <QPointer>

class RelationMaker : public QMainWindow
{
    Q_OBJECT
public:
    explicit RelationMaker(QWidget *parent,
                           QVector<QPointer<NodeRow>> pkList,
                           QVector<QPointer<NodeRow>> fkList);

private:
    QVector<QPair<QString, QPointer<NodeRow>>> pkNamedList, fkNamedList;
    QStringList pkNames, fkNames;

    QPair<QString, QPointer<NodeRow>> currentPkNodeRow, currentFkNodeRow;

    QWidget* getMainWindow();

    void exit();
    void initUI();
    void addNodeRows();
    void insertCurrentNodeRows(QPointer<NodeRow>, QPointer<NodeRow>);
    void nameVectors(QVector<QPointer<NodeRow>> &,
                     QStringList &,
                     QVector<QPair<QString, QPointer<NodeRow>>> &);
    void setCurrentNodeRow(const QString &,
                           QVector<QPair<QString, QPointer<NodeRow>>> &,
                           QPair<QString, QPointer<NodeRow>> &);

    static QString pointerToStr(QPointer<NodeRow>);
};

#endif // RELATIONMAKER_H
