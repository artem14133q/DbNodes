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
    explicit WorkArea(QWidget *parent, QString);

    static const int GET_FK_NODE_ROWS = 2;
    static const int GET_PK_NODE_ROWS = 1;

    void makeRelation(QString, QPointer<NodeRow>, QPointer<NodeRow>);
    void deleteRalation(QString &);
    void setNodeRow(QPointer<NodeRow>);
    void createNodeFromFile(QString, QString, QPoint);

    QString getProjectName();
    QVector<QPointer<Node>> getAllNodes();
    QVector<QPair<QString, QStringList>> getAllrelations();
    QPointer<NodeRow> findNodeRow(const int, QString);

private:
    QVector<QPair<QString, QStringList>> relations;
    QVector<QPair<QString, QPointer<DeleteArrowButton>>> closeBtnList;
    QVector<QPointer<NodeRow>> pkList, fkList;
    QVector<QPointer<Node>> nodeList;
    QString projectName;

    void contextMenuEvent(QContextMenuEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
//    bool event(QEvent *event) override;

    void startRelationMaker();
    void cleanNodeList();
    void createNode(QPoint);
    void cleanNodeRowsList(QVector<QPointer<NodeRow>>&);
};

#endif // WORKAREA_H
