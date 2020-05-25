#ifndef WORKAREA_H
#define WORKAREA_H

#include <QWidget>
#include <node.h>
#include <QWidgetList>
#include <QPointer>
#include <QPushButton>
#include <deletearrowbutton.h>

class WorkArea : public QWidget
{
    Q_OBJECT

public:
    explicit WorkArea(QWidget *parent = nullptr);

    void makeRelation(QString, QPointer<NodeRow>, QPointer<NodeRow>);
    void deleteRalation(QString &);
    void setNodeRow(QPointer<NodeRow>);

private:
    QVector<QPair<QString, QVector<QPointer<NodeRow>>>> relations;
    QVector<QPair<QString, QPointer<DeleteArrowButton>>> closeBtnList;
    QVector<QPointer<NodeRow>> pkList, fkList;

    void contextMenuEvent(QContextMenuEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void startRelationMaker();
    void createNode(QPoint);
    void cleanNodeRowsList(QVector<QPointer<NodeRow>>&);
};

#endif // WORKAREA_H
