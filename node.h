#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QVBoxLayout>
#include <noderow.h>

class Node : public QWidget
{
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr);

    QString getTableName();

private:
    QPoint oldPos;
    QVBoxLayout* rowsLayout;
    QVBoxLayout* pkLayout;
    QVBoxLayout* fkLayout;

    QString tableName = "table";

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

    void initUI();

    void addColoumn(const int);
    void setTableName(const QString &);
};

#endif // NODE_H
